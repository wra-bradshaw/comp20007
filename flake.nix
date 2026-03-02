{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    systems.url = "systems";
  };
  outputs =
    {
      self,
      nixpkgs,
      systems,
    }@inputs:
    let
      forEachSystem = nixpkgs.lib.genAttrs (import systems);
    in
    {
      overlays = {
        default = final: prev: {
        };
      };
      packages = forEachSystem (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ self.overlays.default ];
          };
        in
        {
          default = { };

          projects = {
            debug =
              let
                projectsManifest = builtins.fromJSON (builtins.readFile ./manifest.json);
              in
              (builtins.listToAttrs (
                map (s: {
                  name = s;
                  value = pkgs.stdenv.mkDerivation {
                    name = "${s}-debug";
                    src = ./projects/${s};
                    cmakeFlags = [ "-DCMAKE_BUILD_TYPE=Debug" ];
                    dontStrip = true;
                    buildInputs = [
                      pkgs.criterion
                    ];
                    nativeBuildInputs = [
                      pkgs.cmake
                      pkgs.pkg-config
                    ];
                    meta = {
                      mainProgram = s;
                    };
                  };
                }) projectsManifest
              ));

            release =
              let
                projectsManifest = builtins.fromJSON (builtins.readFile ./manifest.json);
              in
              (builtins.listToAttrs (
                map (s: {
                  name = s;
                  value = pkgs.stdenv.mkDerivation {
                    name = s;
                    src = ./projects/${s};
                    cmakeFlags = [ "-DCMAKE_BUILD_TYPE=Release" ];
                    buildInputs = [
                      pkgs.criterion
                    ];
                    nativeBuildInputs = [
                      pkgs.cmake
                      pkgs.pkg-config
                    ];
                    meta = {
                      mainProgram = s;
                    };
                  };
                }) projectsManifest
              ));
          };

          generate-manifest = pkgs.writeShellApplication {
            name = "generate-manifest";
            runtimeInputs = [
              pkgs.jq
              pkgs.git
            ];
            text = ''
              ROOT_DIRECTORY="$(git rev-parse --show-toplevel)"

              find "$ROOT_DIRECTORY/projects" -mindepth 1 -maxdepth 1 -type d -printf '%P\n' | git check-ignore --stdin --verbose --non-matching | awk '$1 == "::" {print $2}' | jq -R -s 'split("\n")[:-1]' > "$ROOT_DIRECTORY/manifest.json"
            '';
          };

          new-project = pkgs.writeShellApplication {
            name = "new-project";
            runtimeInputs = [
              self.packages.${system}.generate-manifest
              pkgs.git
              pkgs.gnused
            ];
            text = ''
              if [ -z "$1" ]; then
              printf "Error: No directory name provided.\n" >&2
              printf "Usage: %s <directory_name>\n" "$0" >&2
              exit 1
              fi

              ROOT_DIRECTORY="$(git rev-parse --show-toplevel)/projects"
              DIRECTORY="$ROOT_DIRECTORY/$1"

              mkdir "$DIRECTORY"
              cp --no-preserve=mode ${./boilerplate.c} "$DIRECTORY/main.c"
              cp --no-preserve=mode ${./boilerplate_test.c} "$DIRECTORY/test.c"
              sed "s/@PROJECT_NAME@/$1/g" ${./boilerplate_CMakeLists.txt} > "$DIRECTORY/CMakeLists.txt"
              generate-manifest

              git add "$DIRECTORY/main.c" "$DIRECTORY/test.c" "$DIRECTORY/CMakeLists.txt"
              git add "$(git rev-parse --show-toplevel)/manifest.json"
            '';
          };

          debug = pkgs.writeShellApplication {
            name = "debug";
            runtimeInputs = [ pkgs.lldb ];
            text = ''
              PROJECT_NAME="$1"
              if [ -z "$PROJECT_NAME" ]; then
              	echo "Usage: $0 <project-name> [--test]" >&2
              	exit 1
              fi

              if [ "''${2:-}" = "--test" ]; then
                BINARY_PATH="$(nix build --print-out-paths --no-link ".#projects.debug.$PROJECT_NAME")/bin/test"
                echo "$BINARY_PATH"
                lldb "$BINARY_PATH"
              else
                BINARY_PATH="$(nix build --print-out-paths --no-link ".#projects.debug.$PROJECT_NAME")/bin/$PROJECT_NAME"
                echo "$BINARY_PATH"
                lldb "$BINARY_PATH"
              fi
            '';
          };
        }
      );

      devShells = forEachSystem (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ self.overlays.default ];
          };
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              nodejs
              self.packages.${system}.generate-manifest
              self.packages.${system}.new-project
              self.packages.${system}.debug
              cmake
              pkg-config
              criterion
            ];
          };
        }
      );
    };
}

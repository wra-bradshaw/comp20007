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
                  value = pkgs.clangStdenv.mkDerivation {
                    name = "${s}-debug";
                    src = ./projects/${s};
                    mesonBuildType = "debugoptimized";
                    mesonFlags = [
                      "-Db_sanitize=address,undefined"
                      "-Db_lundef=false"
                      "-Dwerror=true"
                    ];
                    dontStrip = true;
                    doCheck = false;
                    buildInputs = [
                      pkgs.criterion
                    ];
                    nativeBuildInputs = [
                      pkgs.meson
                      pkgs.ninja
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
                  value = pkgs.clangStdenv.mkDerivation {
                    name = s;
                    src = ./projects/${s};
                    mesonBuildType = "release";
                    doCheck = false;
                    buildInputs = [
                      pkgs.criterion
                    ];
                    nativeBuildInputs = [
                      pkgs.meson
                      pkgs.ninja
                      pkgs.pkg-config
                    ];
                    meta = {
                      mainProgram = s;
                    };
                  };
                }) projectsManifest
              ));
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
              git
              jq
              lldb
              just
              meson
              ninja
              pkg-config
              criterion
            ];
          };
        }
      );
    };
}

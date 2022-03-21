{
  description = "A presentation about sandboxes";

  inputs = {
    nixpkgs.url = "nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    let
      mergeEnvs = pkgs: envs:
        pkgs.mkShell (builtins.foldl' (a: v: {
          buildInputs = a.buildInputs ++ v.buildInputs;
          nativeBuildInputs = a.nativeBuildInputs ++ v.nativeBuildInputs;
          propagatedBuildInputs = a.propagatedBuildInputs
            ++ v.propagatedBuildInputs;
          propagatedNativeBuildInputs = a.propagatedNativeBuildInputs
            ++ v.propagatedNativeBuildInputs;
          shellHook = a.shellHook + "\n" + v.shellHook;
        }) (pkgs.mkShell { }) envs);
    in flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        version = "0.1.0";

        makeCMakeBuild = name: pkgs.stdenv.mkDerivation {
          pname = name;
          src = ./.;
          inherit version;

          shellHook = "";
          nativeBuildInputs = with pkgs; [
            ninja
            cmake
            extra-cmake-modules
            fmt
          ];
        };
      in rec {
        packages = {
          cpuburn = makeCMakeBuild "cpuburn";
        };
        defaultPackage = packages.cpuburn;

        devShells = {
          application = packages.cpuburn;
          tools = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [
              gdb
            ];
            shellHook = "";
          };
        };
        devShell = mergeEnvs pkgs [devShells.tools devShells.application];
      });
}

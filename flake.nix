{

description = "Example flake with a devShell";

inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

outputs =
{ self, nixpkgs }:
let
    system = "x86_64-linux";

    m_cloc = pkgs.writeShellScriptBin "cloc" ''
        fd -e hpp -e cpp . ./ --exclude external | xargs wc -l
    '';

    run = pkgs.writeShellScriptBin "run" "./run.sh";
    debug = pkgs.writeShellScriptBin "debug" "./run.sh debug";
    clean = pkgs.writeShellScriptBin "clean" "./run.sh clean";

    pkgs = import nixpkgs { inherit system; };
in
{
    devShells.x86_64-linux.default = pkgs.mkShell {
    buildInputs = with pkgs; [
        # build system
        gcc
        cmake
        clang-tools

        # utilities
        m_cloc
        run debug clean
    ];
    shellHook = ''
        echo "Welcome to the devShell!"
    '';
    };
};

}

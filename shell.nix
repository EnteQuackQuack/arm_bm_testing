{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  packages = with pkgs; [
    gcc-arm-embedded
    gdb
    gnumake
    unzip
    pyocd
    bear
  ];
}

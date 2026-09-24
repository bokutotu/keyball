{
  description = "Build this fork's customized Keyball39 firmware";

  inputs = {
    # Keep QMK 0.22.14 on a compatible MILC API and AVR GCC 8 toolchain.
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    qmk = {
      url = "git+https://github.com/qmk/qmk_firmware?ref=refs/tags/0.22.14&shallow=1&submodules=1";
      flake = false;
    };
  };

  outputs = { nixpkgs, qmk, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };

      firmware = pkgs.stdenvNoCC.mkDerivation {
        pname = "keyball39-default";
        version = "qmk-0.22.14";
        src = qmk;

        nativeBuildInputs = [
          pkgs.qmk
          pkgs.python3
          pkgs.git
        ];

        # Use the local fork, not a second checkout of upstream Keyball.
        postPatch = ''
          cp -R ${./qmk_firmware/keyboards/keyball} keyboards/keyball
          chmod -R u+w keyboards/keyball
        '';

        dontConfigure = true;
        hardeningDisable = [ "all" ];

        buildPhase = ''
          runHook preBuild
          export HOME="$TMPDIR/home"
          export QMK_HOME="$PWD"
          mkdir -p "$HOME"

          # Omit wall-clock timestamps and Git metadata from the firmware.
          make -j"$NIX_BUILD_CORES" \
            SKIP_GIT=yes SKIP_VERSION=yes COLOR=false \
            keyball/keyball39:default
          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall
          install -Dm644 keyball_keyball39_default.hex \
            "$out/keyball_keyball39_default.hex"
          runHook postInstall
        '';

        # The output is Intel HEX, not a host executable.
        dontFixup = true;

        meta = {
          description = "Keyball39 firmware with this fork's customized default keymap";
          license = pkgs.lib.licenses.gpl2Plus;
          platforms = [ system ];
        };
      };
    in
    {
      packages.${system}.default = firmware;
      checks.${system}.firmware = firmware;
    };
}

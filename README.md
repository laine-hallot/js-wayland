# JS Wayland

This repo is meant to be a set of Wayland function bindings for JS compiled with Shermes, but its very not unfinished.

The only reason this repo was made public in its current state is because I needed help dealing with what I thought was a Shermes bug (it wasn't I'm just dumb).

## Dependencies

Everything is defined in `./flake.nix` so if you're using Nix you can just run `nix develop` to get your environment set up. If you aren't using nix you can read list of packages defined under `buildInputs` inside `./flake.nix` and then figure out what the equivalent packages are in your package manager.

## Usage

1. `git clone --recursive git@github.com:laine-hallot/js-wayland.git`
1. `npm i`
1. `./build-hermes.sh`
1. `npm -w ./packages/js-wayland run build` - build the JS Wayland and a executable that shows an example window (well that last part is broken)
1. `./packages/js-wayland/dist/run.o` - run the application

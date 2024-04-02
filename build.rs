extern crate bindgen;
extern crate cc;

use std::{env, path::PathBuf};

use bindgen::CargoCallbacks;

fn main() {
    // Compile the C library
    let files = [
        "libfli-camera.c",
        "libfli-camera-parport.c",
        "libfli-camera-usb.c",
        "libfli-filter-focuser.c",
        "libfli-mem.c",
        "libfli-raw.c",
        "unix/libfli-usb.c",
        "unix/libfli-debug.c",
        "unix/libfli-serial.c",
        "unix/libfli-sys.c",
        "unix/libusb/libfli-usb-sys.c",
    ];

    let files = files
        .iter()
        .map(|f| {
            format!("clib/{}", f)
                .parse::<PathBuf>()
                .expect("Cannot parse path")
        })
        .collect::<Vec<PathBuf>>();

    cc::Build::new()
        .opt_level(3)
        .flag("-pthread")
        .flag("-D__LIBUSB__")
        .files(files)
        .include("clib")
        .include("clib/unix")
        .compile("libfli-usb.a");
    // This is the directory where the `c` library is located.
    // Canonicalize the path as `rustc-link-search` requires an absolute path.
    let libdir_path = PathBuf::from("clib")
        .canonicalize()
        .expect("cannot canonicalize path");

    // This is the path to the `c` headers file.
    let headers_path = libdir_path.join("libfli.h");

    let headers_path_str = headers_path.to_str().expect("Path is not a valid string");

    // Tell cargo to look for shared libraries in the specified directory
    println!("cargo:rustc-link-search={}", libdir_path.to_str().unwrap());

    // Tell cargo to tell rustc to link our `hello` library. Cargo will
    // automatically know it must look for a `libhello.a` file.
    println!("cargo:rustc-link-lib=fli-usb");
    println!("cargo:rustc-link-lib=pthread");
    println!("cargo:rustc-link-lib=rt");
    println!("cargo:rustc-link-lib=m");
    println!("cargo:rustc-link-lib=usb-1.0");

    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let bindings = bindgen::Builder::default()
        // The input header we would like to generate
        // bindings for.
        .header(headers_path_str)
        // Tell cargo to invalidate the built crate whenever any of the
        // included header files changed.
        .parse_callbacks(Box::new(CargoCallbacks::new()))
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());

    let out_path = out_path.join("bindings.rs");

    bindings
        .write_to_file(out_path)
        .expect("Couldn't write bindings!");
}

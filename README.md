edid-generator
==============

Hackerswork to generate an EDID binary file from given Xorg Modelines

An extension of the awesome work provided in the Linux kernel documentation (in `docs/EDID`).

Simplifies the process greatly by allowing you to use a standard modeline as well as automatically calculating the CRC
and applying it to the resulting image.

Requirements
------------

```
sudo apt install zsh edid-decode automake dos2unix
```

Usage
-----

If you don't have a `<mode>.S` prepared yet, generate one using a file containing Xorg Modelines. Lines that do not
contain modelines are ignored, so you can just read right from `xorg.conf`.

```s
./modeline2edid /etc/X11/xorg.conf
```

You can also just read from `stdin` the way you'd expect:

```s
./modeline2edid
# or explicitly:
./modeline2edid -
```

After this creates your `<name>.S` files for each modeline it encounters, simply `make`:

```sh
make
```

The end result, providing all goes well, is a glorious EDID bin image for each mode you gave to it. A `<name>.S` file
is templated, and then `make` is invoked to compile it into `<name>.bin`. It's actually compiled twice; once with an
invalid CRC in order to generate said CRC to enter it into the template, after which we recompile, hence glorious bins.

NOTE: If you use a ratio other than 16:9, you'll need to specify it at the end of the modeline.as `ratio=4:3`.
Ratios are hard defined in `edid.S`, so if you are trying to do something non-standard you'll need to add it.

Why?
----

Many monitors and TVs (both high and low end) provide invalid EDID data. After dealing with this for years, I wanted to
automate this process.

The final straw was when I bought a cheap Sceptre 4K tv at a rather affordable ~$225 and ran into a long series of hurdles to get it to operate
as expected at `3840x2160@60`. After doing this enough times, I had to automate it or I was going to go crazy.

I used this to quickly iterate while troubleshhooting, finally it's all working from KMS all the way down to X!

(Via `drm_kms_helper.edid_firmware=DP-1:edid/blah.bin` if you're interested. I'm using radeon + intel, with nvidia you
have to specify it in `xorg.conf`/`xorg.conf.d` as they don't yet support KMS for the fb console yet; their beta
drivers, 367 at the time of writing, only support using KMS for the xorg server.)

Sometimes I hate being such a perfectionist. Keep in mind this project was made in a couple hours, I certainly didn't
attempt to polish it in the least ;)

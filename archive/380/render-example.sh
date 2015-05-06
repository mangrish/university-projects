#!/bin/sh
#
# This bash script is an example test case for your project. Run it and
# redirect the output to xv:
#
#   render-example.sh | xv -
#

#./proj 2 4 4 <<EOF
./proj 512 1024 1024 <<EOF
#./proj 128 256 256 <<EOF
off ./ktests/krl-cube.off 1.0 1.0 1.0 10.0 -10.0 0.0 3.0 3.0 -6.0
light 1.0 -1.0 0.0 0.0
off ./ktests/head.off 0.2 0.2 0.2 20.0 20.0 0.0 3.0 -4.0 -6.0
ambient 0.3
off ./ktests/seashell.off 4.0 4.0 4.0 -30.0 30.0 0.0 -3.0 -3.0 -6.0
light 1.0 0.0 1.0 0.0
off ./ktests/x29_plane.off 0.1 0.1 0.1 45.0 0.0 180.0 -3.0 3.0 -6.0
light 1.0 1.0 -1.0 1.0
EOF

#!/usr/bin/perl

use strict;
use POSIX;
use GD;
use IO::Uncompress::Gunzip qw(gunzip $GunzipError);
use File::Basename;

my $empty = 0;

if ($ARGV[0] eq '') {
  print "map2png.pl <map_####.dat>\n";
  exit;
}
my $fn = $ARGV[0];

my $fh = new IO::File "<$fn" || die "Cannot open $fn: $!\n" ;
my $raw;
gunzip $fh => \$raw || die "gunzip failed: $GunzipError\n";
undef $fh;

my $cpos = 10 + index $raw, 'colors';
my $cdata = substr $raw, $cpos, 16384;
my $clen = length $cdata;

my $img = new GD::Image(128,128,1);

# build color map
my @c;

my %baseCol = (
  0  => [127, 0, 255],
  1  => [127, 178, 56],
  2  => [247, 233, 163],
  3  => [199, 199, 199],
  4  => [255, 0, 0],
  5  => [160, 160, 255],
  6  => [167, 167, 167],
  7  => [0, 124, 0],
  8  => [255, 255, 255],
  9  => [164, 168, 184],
  10 => [151, 109, 77],
  11 => [112, 112, 112],
  12 => [64, 64, 255],
  13 => [143, 119, 72],
  14 => [255, 252, 245],
  15 => [216, 127, 51],
  16 => [178, 76, 216],
  17 => [102, 153, 216],
  18 => [229, 229, 51],
  19 => [127, 204, 25],
  20 => [242, 127, 165],
  21 => [76, 76, 76],
  22 => [153, 153, 153],
  23 => [76, 127, 153],
  24 => [127, 63, 178],
  25 => [51, 76, 178],
  26 => [102, 76, 51],
  27 => [102, 127, 51],
  28 => [153, 51, 51],
  29 => [25, 25, 25],
  30 => [250, 238, 77],
  31 => [92, 219, 213],
  32 => [74, 128, 255],
  33 => [0, 217, 58],
  34 => [129, 86, 49],
  35 => [112, 2, 0],
  36 => [209, 177, 161],
  37 => [159, 82, 36],
  38 => [149, 87, 108],
  39 => [112, 108, 138],
  40 => [186, 133, 36],
  41 => [103, 117, 53],
  42 => [160, 77, 78],
  43 => [57, 41, 35],
  44 => [135, 107, 98],
  45 => [87, 92, 92],
  46 => [122, 73, 88],
  47 => [76, 62, 92],
  48 => [76, 50, 35],
  49 => [76, 82, 42],
  50 => [142, 60, 46],
  51 => [37, 22, 16]
);

my @mult = qw(180 220 255 135);

sub createPalette {

  my %palette;

  foreach my $i (sort {$a<=>$b} keys %baseCol) {
    for (my $j = 0; $j < 4; $j++) {
      my $id = $i * 4 + $j;
      $palette{$id} = [
        floor($baseCol{$i}[0] * $mult[$j] / 255),
        floor($baseCol{$i}[1] * $mult[$j] / 255),
        floor($baseCol{$i}[2] * $mult[$j] / 255)
      ];
    }
  }

  return %palette;

}

my %p = createPalette();
foreach my $i (sort {$a<=>$b} keys %p) {
  $c[$i] = $img->colorAllocate ($p{$i}[0], $p{$i}[1], $p{$i}[2]);
}

if ($cdata) {
  for (my $y = 0; $y <= 127; $y++) {
    for (my $x = 0; $x <= 127; $x++) {
      my $cid = unpack ('C',substr ($cdata, $y * 128 + $x, 1));
      # print "$cid ";
      $img->setPixel ($x, $y, $c[$cid]);
      $empty++ if ($cid == 0);
    }
  }
  printf "Unplotted pixels: %d/%d\nMap completion: %0.2f%%\n",
         $empty, $clen, 100 * ($clen - $empty) / $clen;
  my $png = $img->png;
  my ($base,$path,$ext) = fileparse($fn,'.dat');
  my $pf = $path . $base . ".png";
  open my $ph, ">", $pf || die "unable to save image $pf: $!\n";
  binmode $ph;
  print $ph $png;
  close $ph;
  print "created image $pf\n";
} else {
  print "failed to read file!";
  exit;
}

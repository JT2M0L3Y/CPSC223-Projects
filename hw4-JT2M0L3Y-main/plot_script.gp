#---------------------------------------------------------------------------
# This is a basic gnuplot script to generate a performance comparison
# graph. This example requires an "output.dat" file, which is
# generated by running:
#
#     ./hw4_perf > output.dat
#
# To run this script type the following at the command line
#
#     gnuplot -c plot_script.gp
#
# After running the above two commands, the files fast-sort-perf.png and
# slow-sort-perf.png will be generated.
#---------------------------------------------------------------------------

infile = "output.dat"
outfile1 = "fast-sort-perf.png"
outfile2 = "slow-sort-perf.png"

# Set the terminal for PNG output and aspect ratio
set terminal pngcairo enhanced font 'Verdana, 10'
set size ratio 0.75

# Set the title and each axis label
set xlabel "Input Size (n)"
set ylabel "Time (msec)"

# Move the key to the left of the graph
set key left

# color scheme
RED = "#e6194B"
GREEN = "#3cb44b"
YELLOW = "#ffe119"
BLUE = "#4363d8"
ORANGE = "#f58231"
PURPLE = "#911eb4"
CYAN = "#42d4f4"
MAGENTA = "#f032e6"
LIME = "#bfef45"
PINK = "#fabed4"
TEAL = "#469990"
LAVENDER = "#dcbeff"
BROWN = "#9A6324"
BEIGE = "#fffac8"
MAROON = "#800000"
MINT = "#aaffc3"
OLIVE = "#808000"
APRICOT = "#ffd8b1"
NAVY = "#000075"
GREY = "#a9a9a9"

set xtics 1500

# Plot the "fast" data
set output outfile1
set title "LinkedSeq vs ArraySeq Sort Performance (Fast)";
plot  infile u 1:2 t "ArraySeq Merge Sort, Reversed" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:3 t "ArraySeq Merge Sort, Shuffled" w linespoints lw 3 lc rgb GREEN pointtype 6, \
      infile u 1:5 t "ArraySeq Quick Sort, Shuffled" w linespoints lw 2 lc rgb BLUE pointtype 6, \
      infile u 1:6 t "ArraySeq Quick Random, Reversed" w linespoints lw 2 lc rgb ORANGE pointtype 6, \
      infile u 1:7 t "ArraySeq Quick Random, Shuffled" w linespoints lw 2 lc rgb PURPLE pointtype 6, \
      infile u 1:8 t "LinkedSeq Merge Sort, Reversed" w linespoints lw 2 lc rgb CYAN pointtype 6, \
      infile u 1:9 t "LinkedSeq Merge Sort, Shuffled" w linespoints lw 2 lc rgb MAGENTA pointtype 6, \
      infile u 1:11 t "LinkedSeq Quick Sort, Shuffled" w linespoints lw 2 lc rgb LIME pointtype 6, \
      infile u 1:12 t "LinkedSeq Quick Random, Reversed" w linespoints lw 2 lc rgb PINK pointtype 6, \
      infile u 1:13 t "LinkedSeq Quick Random, Shuffled" w linespoints lw 2 lc rgb TEAL pointtype 6;
      
# Plot the "slow" data
set output outfile2
set title "LinkedSeq vs ArraySeq Sort Performance (Slow)";
plot   infile u 1:4 t "ArraySeq Quick Sort, Reversed" w linespoints lw 3 lc rgb RED pointtype 6, \
       infile u 1:6 t "ArraySeq Quick Random, Reversed" w linespoints lw 2 lc rgb GREEN pointtype 6, \
       infile u 1:10 t "LinkedSeq Quick Sort, Reversed" w linespoints lw 2 lc rgb BLUE pointtype 6, \
       infile u 1:12 t "LinkedSeq Quick Random, Reversed" w linespoints lw 2 lc rgb ORANGE pointtype 6;



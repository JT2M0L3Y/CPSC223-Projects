#---------------------------------------------------------------------------
# This is a basic gnuplot script to generate a performance comparison
# graph. This example requires an "output.dat" file, which is
# generated by running:
#
#     ./hw10_perf > output.dat
#
# To run this script type the following at the command line
#
#     gnuplot -c plot_script.gp
#
# For example, given output.dat, run the following from the command
# line:
#
#     gnuplot -c plot_script.gp
#---------------------------------------------------------------------------

infile = "output.dat"
outfile1 = "insert_graph.png"
outfile2 = "erase_graph.png"
outfile3 = "contains_graph.png"
outfile4 = "find_range_graph.png"
outfile5 = "next_key_graph.png"
outfile6 = "sorted_keys_graph.png"
outfile7 = "tree_stats.png"

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

# Set the terminal for PNG output and aspect ratio
set terminal pngcairo enhanced font 'Verdana, 10'
set size ratio 0.75

# Set the title and each axis label
set xlabel "Input Size (n)"
set ylabel "Time (millisec)"

# Move the key to the left of the graph
set key left

# Turn on the grid
set grid

# Change x-axis to print large numbers
set auto x
set xtics rotate
set xtics 0,100000,1000000

#----------------------------------------------------------------------
# Save the graph
set output outfile1

# Change y-axis to zero or higher
set yrange [0:0.1] noreverse writeback

# Plot the data
set title "AVL vs BTree Map Insert Performance";
plot  infile u 1:2 t "AVLMap Insert" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:3 t "BTreeMap Insert" w linespoints lw 3 lc rgb GREEN pointtype 6.


#----------------------------------------------------------------------
# Save the graph
set output outfile2

# Change y-axis to zero or higher
set yrange [0:0.1] noreverse writeback

# Plot the data
set title "AVL vs BTree Map Erase Performance";
plot  infile u 1:4 t "AVLMap Erase" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:5 t "BTreeMap Erase" w linespoints lw 3 lc rgb GREEN pointtype 6.


#----------------------------------------------------------------------
# Save the graph
set output outfile3

# Change y-axis to zero or higher
set yrange [0:0.1] noreverse writeback

# Plot the data
set title "BinSearch vs Hash vs BST vs AVL Map Contains Performance";
plot  infile u 1:6 t "AVLMap Contains" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:7 t "BTreeMap Contains" w linespoints lw 3 lc rgb GREEN pointtype 6.


#----------------------------------------------------------------------
# Save the graph
set output outfile4

# Change y-axis to zero or higher
set yrange [0:*] noreverse writeback

# Plot the data
set title "AVL vs BTree Map Find Range Performance";
plot  infile u 1:8 t "AVLMap Find Range" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:9 t "BTreeMap Find Range" w linespoints lw 3 lc rgb GREEN pointtype 6.


#----------------------------------------------------------------------
# Save the graph
set output outfile5

# Change y-axis to zero or higher
set yrange [0:0.1] noreverse writeback

# Plot the data
set title "AVL vs BTree Map Next Key Performance";
plot  infile u 1:10 t "AVLMap Next Key" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:11 t "BTreeMap Next Key" w linespoints lw 3 lc rgb GREEN pointtype 6.


#----------------------------------------------------------------------
# Save the graph
set output outfile6

# Change y-axis to zero or higher
set yrange [0:*] noreverse writeback

# Plot the data
set title "AVL vs BTree Map Sorted Keys Performance";
plot  infile u 1:12 t "AVLMap Sorted Keys" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:13 t "BTreeMap Sorted Keys" w linespoints lw 3 lc rgb GREEN pointtype 6.


#----------------------------------------------------------------------
# Save the graph
set output outfile7

set ylabel "Tree Height"
set yrange [0:30] noreverse writeback

set title "AVL and BTree Height vs lg Growth";
plot  infile u 1:14 t "AVL Height" w linespoints lw 3 lc rgb RED pointtype 6, \
      infile u 1:15 t "BTree Height" w linespoints lw 3 lc rgb GREEN pointtype 6, \
      infile u 1:16 t "lg n" w linespoints lw 3 lc rgb BLUE pointtype 6.


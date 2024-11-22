from matplotlib.ticker import FormatStrFormatter
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
import subprocess as sp
import argparse
import sys

parser = argparse.ArgumentParser( 
   description='Script for graphing bandwidth plots from the SHMEM Perf Suite.')

parser.add_argument('--input', '-i', metavar='FILENAME', type=str, nargs='+',
                    required=True,
                    help='list of log filenames to read and plot (required)')
parser.add_argument('--title', '-t', metavar='TITLE', type=str, required=False,
                    help='String for the title of the generated plot (optional)')
parser.add_argument('--maximum', '-m', required=False, action='store_true',
                    help='Plot the maximum y-value across all input files (optional)')
parser.add_argument('--csv', '-c', required=False, action='store_true',
                    help='Read simple CSV files with the -i flag (optional)')
parser.add_argument('--message-rate', '-r', required=False, action='store_true',
                    help='Plot message rate instead of throughput (optional)')
parser.add_argument('--dpi', '-d', metavar='DPI', type=int, default=200, required=False,
                    help='The dots per inch (DPI) of the generated plot')

args = parser.parse_args()

filenames = args.input

lines = []
matrices = []
max_matrices = []
found_bytes = False

import pdb; pdb.set_trace()

if args.csv:
   for i in range(len(filenames)):
       matrices.append( np.loadtxt(filenames[i], delimiter=',') )

elif not args.maximum:
   # Iterate through all input files and create temporary .data file with clean
   # (space delimited) data and pass that through numpy.loadtxt().
   for i in range(len(filenames)):
       lines.append( open(filenames[i]).readlines() )
       data_filename = 'generated_' + filenames[i] + '.data'
       csv_filename = 'generated_' + filenames[i] + '.csv'
       for j in range(len(lines[i])):
           if lines[i][j].find('bytes') != -1:
               found_bytes = True
               break
       if found_bytes == False: # no bytes found, so this is a header line
           j = -1 # index j==0 begins the header
       #Check whether the next line starts with a number/digit:
       elif not lines[i][j+1].split()[0].isdigit():
           import pdb; pdb.set_trace()
           print("Unexpected value, quitting. Please check file formatting.")
           sys.exit(-1)
   
       with open(data_filename, 'w') as file:
           file.writelines(lines[i][j+1:])
   
       with open(csv_filename, 'w') as file:
           for k in range(len(lines[i][j+1:])):
               file.write(', '.join(lines[i][j+1:][k].split()) + '\n')
   
       matrices.append( np.loadtxt(data_filename) )
       sp.check_call('rm ' + data_filename, shell=True)
   
       # Throughput in MB/s:
       matrices[i][:,1] = matrices[i][:,1]

       # Message rate in millions per second:
       matrices[i][:,2] = matrices[i][:,2]/1000000.

else:
   # Iterate through all input files and create temporary .data file with clean
   # (space delimited) data and pass that through numpy.loadtxt().
   for i in range(len(filenames)):
       lines.append( open(filenames[i]).readlines() )
       data_filename = 'generated_' + filenames[i] + '.data'
       csv_filename = 'generated_' + filenames[i] + '.csv'
       for j in range(len(lines[i])):
           if lines[i][j].find('bytes') != -1:
               found_bytes = True
               break
       if found_bytes == False: # no bytes found, so this is a header line
           j = -1 # index j==0 begins the header
       #Check whether the next line starts with a number/digit:
       elif not lines[i][j+1].split()[0].isdigit():
           import pdb; pdb.set_trace()
           print("Unexpected value, quitting. Please check file formatting.")
           sys.exit(-1)
   
       with open(data_filename, 'w') as file:
           file.writelines(lines[i][j+1:])
   
       with open(csv_filename, 'w') as file:
           for k in range(len(lines[i][j+1:])):
               file.write(', '.join(lines[i][j+1:][k].split()) + '\n')
   
       matrices.append( np.loadtxt(data_filename) )
       if i == 0:
          max_matrices.append( np.loadtxt(data_filename) )
          max_matrices[0][:,2] = max_matrices[0][:,2]/1000000.
       sp.check_call('rm ' + data_filename, shell=True)
   
       # Throughput in MB/s:
       matrices[i][:,1] = matrices[i][:,1]

       # Message rate in millions per second:
       matrices[i][:,2] = matrices[i][:,2]/1000000.

       for k in range(len(matrices[i])):
           max_matrices[0][k,1] = max(max_matrices[0][k,1], matrices[i][k,1])
           max_matrices[0][k,2] = max(max_matrices[0][k,2], matrices[i][k,2])

   with open("generated_MAX.csv", 'w') as file:
       for k in range(len(max_matrices[0])):
           file.write(str(max_matrices[0][k,0]) + ', ' + str(max_matrices[0][k,2]) + '\n')


plt.rc('xtick', labelsize=11)  # fontsize of the tick labels
plt.rc('ytick', labelsize=11)  # fontsize of the tick labels 

plt.rc('axes', labelsize=13)  # fontsize of the tick labels
plt.rc('axes', titlesize=13)  # fontsize of the tick labels 

fig = plt.figure(dpi=args.dpi)#, figsize=(10,6))

if args.title:
    fig.suptitle(args.title, fontsize=20, fontweight='bold')

ax = fig.add_subplot(111)

ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FormatStrFormatter('%d'))

xlabels = ax.get_xticklabels()
plt.setp(xlabels, rotation=45, fontsize=10)

# Comment these 2 lines if using a non-default message size range:
plt.xticks([1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
            16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152,
            4194304, 8388608], size='small')
if args.maximum:
    assert(len(max_matrices[0][:,0]) == len([1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
       1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
       1048576, 2097152, 4194304, 8388608]))
else:
    for i in range(len(filenames)):
        assert(len(matrices[i][:,0]) == len([1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
           1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
           1048576, 2097152, 4194304, 8388608]))

plt.xlabel('Buffer Size (bytes)', fontsize=13)

ax.set_yscale('log', base=2)

ax.yaxis.set_major_formatter(FormatStrFormatter('%.1f'))

plots = []
colors = ['o-b','+-r','v-g','^-y','*-c','o-m','+-k', 'v-b','^-r','*-g',
          'o-y','+-c','v-m','^-k', '*-b','o-r','+-g','v-y','^-c','*-m','o-k']

if args.message_rate:
    column = 2
    location = 'lower left'
    ax.set_ylabel('Message Rate (millions/sec)', fontsize=13)
else:
    column = 1
    location = 'lower right'
    ax.set_ylabel('Bandwidth (MB/s)', fontsize=13)

if not args.maximum:
    for i in range(len(filenames)):
        plots.append( ax.plot(matrices[i][:,0], matrices[i][:,column], colors[i], fillstyle='none') )
else:
    plots.append( ax.plot(max_matrices[0][:,0], max_matrices[0][:,column], colors[0]) )

l1 = plt.legend([p[0] for p in plots], tuple(filenames),
                loc=location, prop={'size':9})

plt.tight_layout()

plt.savefig('plot.png', dpi=args.dpi)

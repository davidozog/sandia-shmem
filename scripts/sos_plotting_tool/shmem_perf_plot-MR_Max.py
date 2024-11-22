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
                    help='string for the title of the generated plot (optional)')
parser.add_argument('--maximum', '-m', metavar='MINIMUM', type=str, required=False,
                    help='only plot the maximum (y-value) across input data files (optional)')
parser.add_argument('--csv', '-c', metavar='CSV', type=str, required=False,
                    help='Read simple CSV files from -i flag (optional)')

args = parser.parse_args()

filenames = args.input

lines = []
matrices = []
max_matrices = []
found_bytes = False

import pdb; pdb.set_trace()

# TODO: If SOS printed in simple CSV format, could we remove the following?
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
   
       # GB/s:
       #matrices[i][:,1] = matrices[i][:,1]/1000.
       # MB/s:
       matrices[i][:,1] = matrices[i][:,1]
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
   
       # GB/s:
       #matrices[i][:,1] = matrices[i][:,1]/1000.
       # MB/s:
       matrices[i][:,1] = matrices[i][:,1]
       matrices[i][:,2] = matrices[i][:,2]/1000000.

       for k in range(len(matrices[i])):
           max_matrices[0][k,1] = max(max_matrices[0][k,1], matrices[i][k,1])
           max_matrices[0][k,2] = max(max_matrices[0][k,2], matrices[i][k,2])

   with open("MR_max.csv", 'w') as file:
       for k in range(len(max_matrices[0])):
           file.write(str(max_matrices[0][k,0]) + ', ' + str(max_matrices[0][k,2]) + '\n')
       #for k in range(len(lines[i][j+1:])):
       #    file.write(', '.join(lines[i][j+1:][k].split()) + '\n')


# dpi=300 is a little over-the-top, but publication-ready
fig = plt.figure(dpi=300)#, figsize=(10,6))

plt.rc('xtick', labelsize=11)  # fontsize of the tick labels
plt.rc('ytick', labelsize=11)  # fontsize of the tick labels 

plt.rc('axes', labelsize=13)  # fontsize of the tick labels
plt.rc('axes', titlesize=13)  # fontsize of the tick labels 

if args.title:
    fig.suptitle(args.title, fontsize=20, fontweight='bold')

ax = fig.add_subplot(111)

ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FormatStrFormatter('%d'))

xlabels = ax.get_xticklabels()
plt.setp(xlabels, rotation=45, fontsize=10)

# Comment these 2 lines if using a non-default message size range:
plt.xticks([2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
            16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152,
            4194304, 8388608], size='small')
assert(len(matrices[i][:,0]) == len([1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
       1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
       1048576, 2097152, 4194304, 8388608]))

plt.xlabel('Buffer Size (bytes)', fontsize=13)

#ax.set_ylabel('Bandwidth (MB/s)', fontsize=13)
ax.set_ylabel('Message Rate (millions/sec)', fontsize=13)
#ax.set_yscale('log', base=2)

ax.yaxis.set_major_formatter(FormatStrFormatter('%.1f'))

plots = []
colors = ['o-b','+-r','v-g','^-y','*-c','o-m','+-k', 'v-b','^-r','*-g',
          'o-y','+-c','v-m','^-k', '*-b','o-r','+-g','v-y','^-c','*-m','o-k']

if not args.maximum:
    for i in range(len(filenames)):
        # TODO: support message rate replacing 1 with 2 here (and adjust y axis)
        plots.append( ax.plot(matrices[i][1:,0], matrices[i][1:,1], colors[i], fillstyle='none') )
else:
    # TODO: support message rate replacing 1 with 2 here (and adjust y axis)
    plots.append( ax.plot(max_matrices[0][:,0], max_matrices[0][:,2], colors[i]) )

l1 = plt.legend([p[0] for p in plots], tuple(filenames),
                loc="upper right", prop={'size':9})

plt.tight_layout()

plt.savefig('plot.png', dpi=300)

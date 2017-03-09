<p align="center">
  <img src="doc/img/apparatus.png" alt="apparatus logo" width="200px"/>
</p>
<h1 align="center">Apparatus</h1>
<p align="center">Long Read Deep Learning Tools</p>
***
## INSTALL
### Requirements
C++17 compiler, cmake, boost libs, ninja, python3, tensorflow, keras, theano, MAFFT

### Build
    git clone https://github.com/armintoepfer/apparatus.git && cd apparatus
    git submodule update --init --remote --recursive
    mkdir build && cd build
    cmake -GNinja .. && ninja

## RUN
### Train
Cluster your reads of interest by genomic location or molecule, for the latter

    ./extract_subreads movie.subreads.bam

it creates a subreads/ folder containing a folder for each observed coverage
with the respective fasta files.

For each fasta file concat it with the correct sequence, as the first sequence
and create a multiple sequence alignment in msa/:

    for i in subreads/*/*; do
        cat ref.fasta $i > in
        mafft --maxiterate 1000 --localpair --adjustdirection --thread 4 in > msa/${${i##*/}%.*}.fasta
    done

Create a training file for the NN:

    ./mafft_pileup msa/* 2> training

Train the NN and get an estimate of the accuracy:

    python scripts/consensus.py
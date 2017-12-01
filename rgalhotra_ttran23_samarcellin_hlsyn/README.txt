# Author: Rohin Galhotra (574a), Stephanie Marcellin (574a), Tam Tran (474a)
# NetID: rgalhotra, samarcellin, ttran23
# Date: November 2017
# Assignment 2 : hlsyn

(USAGE: hlsyn cFile latency verilogFile)

Project reads in C-like files and uses the provided latency to create a force-directed graph scheduled HLSM.

Rohin: parsing (handle duplicate entries) output file, asap/alap (scrapped), states, succ force
Stephanie: parsing (IO/operations), asap/alap (version we went with), probability distribution, pred force (modified version), total force
Tam: parsing (operations), asap/alap (scrapped), self force, pred force (early version), fds total force scheduling

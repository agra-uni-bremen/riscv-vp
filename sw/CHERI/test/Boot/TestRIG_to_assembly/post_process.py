instr_list = []

with open('benchmark_1000_1.S', 'r') as infile:
    for line in infile:
        splitLine = line.split()

        #print(curr_instr)
        if splitLine != []:
        	instr_list.append(splitLine)

outfile = open('benchmark_1000_1_new.S', 'w')
repeat = 1

for instr in instr_list:
	#print(instr)
	if instr[0] == 'sc.cap':
		controlReg = instr[2].replace("(", "").replace(")", "") + ","
		#print(controlReg)
		print("\n#sccap_change", file=outfile)
		print("auipcc", controlReg, "0", file=outfile)
		print("li", "t5,", "8000", file=outfile)
		print("csetaddr", controlReg, controlReg, "t5", file=outfile)
		print("#sccap_change\n", file=outfile)
		for i in range(repeat):
			print(' '.join(instr), file=outfile)
		
	elif instr[0] == 'sc.ddc':
		controlReg = instr[2].replace("(", "").replace(")", "") + ","
		print("\n#scddc_change", file=outfile)
		print("auipcc", "cs2,", "0", file=outfile)
		print("li", "t5,", "8000", file=outfile)
		print("csetaddr", "cs2,", "cs2,", "t5", file=outfile)
		print("cspecialw", "ddc,", "cs2", file=outfile)
		print("andi", controlReg, controlReg, "64", file=outfile)
		print("#scddc_change\n", file=outfile)
		for i in range(repeat):
			print(' '.join(instr), file=outfile)
		
	elif instr[0] == 'ebreak':
		print("#", instr[0], file=outfile)
	
	elif instr[0] == 'cjal':
		if(len(instr) < 3):
			instr[1] = "4"
		else:
			instr[2] = "4"
		print(' '.join(instr), file=outfile)
		
	elif (instr[0] == 'cjalr') or (instr[0] == 'cjr'):
		if (len(instr) < 3):
			controlReg = instr[1]
		else:
			controlReg = instr[2].replace(",","")
			
		controlReg += ","
		print("\n#cjalr_change", file=outfile)
		print("auipcc", controlReg, "0", file=outfile)
		print("#cjalr_change\n", file=outfile)
		
		if (len(instr) == 4):
			instr[3] = "8"
			print(' '.join(instr), file=outfile)
		else:
			instr.append(", 8")
			print(' '.join(instr), file=outfile)
			
	elif (instr[0] == 'jalr.cap') or (instr[0] == 'jr.cap'):
		if (len(instr) < 3):
			controlReg = instr[1]
		else:
			controlReg = instr[2]
			
		controlReg += ","
		print("\n#jalr.cap_change", file=outfile)
		print("auipcc", controlReg, "0", file=outfile)
		print("cincoffsetimm", controlReg, controlReg, "12", file=outfile)
		print("#jalr.cap_change\n", file=outfile)
		
		print(' '.join(instr), file=outfile)
		
	elif (instr[0] == 'jalr.pcc') or (instr[0] == 'jr.pcc'):
		if (len(instr) < 3):
			controlReg = instr[1]
		else:
			controlReg = instr[2]
			
		controlReg += ","
		print("\n#jalr.pcc_change", file=outfile)
		print("auipcc", "cs2,", "0", file=outfile)
		print("cincoffsetimm", "cs2,", "cs2", "16", file=outfile)
		print("cgetoffset", controlReg, "cs2", file=outfile)
		print("#jalr.pcc_change\n", file=outfile)
		
		print(' '.join(instr), file=outfile)
		
	elif instr[0].startswith('b'):
		instr[3] = "4"
		print(' '.join(instr), file=outfile)
		
	else:
		print(' '.join(instr), file=outfile)
		
outfile.close()

        
            

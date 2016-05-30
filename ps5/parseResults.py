from collections import defaultdict


d = defaultdict(list)
f = open("results")
output = open("data", "w")

hashFunc = None
loadFactor = None
insertion = None
query = None
func = None
for line in f:
	if "####" in line:
		if "######" in line:
			continue

		newLine = line.split()[1:]
		start = newLine.index("Timing")
		end = newLine.index("####")
		hashFunc = "-".join(newLine[start + 1: end])
	if "===" in line:
		func = " ".join(line.split()[1:-1])	

	if "Load Factor" in line:
		index = line.split().index("Factor:") 	
		loadFactor = line.split()[index + 1]
	if "Insertion:" in line:
		index = line.split().index("Insertion:")
		insertion = line.split()[index + 1]
	if "Query:" in line:
		index = line.split().index("Query:")
		query = line.split()[index + 1]
		d[func].append((hashFunc, loadFactor, insertion, query))

for func, arr in d.items():
	output.write(": ".join([func, "Insertion Times"]))
	output.write("\n")
	for hashType, loadFactor, insertion, query in arr:
		output.write(" ".join([hashType, loadFactor, insertion, "\n"]))
	output.write("\n")

	output.write(": ".join([func, "Query Times"]))
	output.write("\n")
	for hashType, loadFactor, insertion, query in arr:
		output.write(" ".join([hashType, loadFactor, query, "\n"]))
	output.write("\n")


output.close()
f.close()

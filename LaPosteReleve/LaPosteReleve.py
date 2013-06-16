#!/usr/bin/python

import glob
import csv
import re


allCsv = "./result/allCsv.csv";

prog = re.compile("(\d{2})/(\d{2})\/(\d{4}).*")
progAmount = re.compile("^-")

result = {}
mycalendar = {
    '01':'January',
    '02':'February',
    '03':'March',
    '04':'April',
    '05':'May',
    '06':'June',
    '07':'Juilly',
    '08':'August',
    '09':'September',
    '10':'October',
    '11':'November',
    '12':'December'

}


statementDate = {}

# Create data structure for new csv file
for file in glob.glob("*.csv"):
    csvReader = csv.reader(open(file), delimiter=';', quotechar='|')
    
    #initialize the date of the statement
    d = 0

    #for line in [ x for x in csvReader if len(x) == 4 and prog.match(x[0])]:
    for line in csvReader:
	if len(line) == 4 and prog.match(line[0]):
	    mydate = line[0].split('/')
	    mydate = "".join([mydate[2],mydate[1],mydate[0]])
	    if mydate not in result:
		result[mydate] = []
	    result[mydate].append(";".join(line))

	if len(line) == 2 and line[0].strip() == 'Date':
	    tmp = line[1].split('/')
	    d = "".join([tmp[2],tmp[1],tmp[0]])
	    if d not in statementDate:
		statementDate[d] = {}

	if len(line) == 2 and line[0].strip() == 'Solde (EUROS)':
	    statementDate[d]['euros'] =  line[1] ;

	if len(line) == 2 and line[0].strip() == 'Solde (FRANCS)':
	    statementDate[d]['francs'] =  line[1];



f = open(allCsv,"wr+")
csvReader = csv.reader(open(allCsv), delimiter=';', quotechar='|')

a = sorted(statementDate.keys(),result.get(0), reverse=True)

euros = statementDate[a[0]]['euros']
francs = statementDate[a[0]]['francs']
 
for newdate in sorted(result.keys(),result.get(0), reverse=True):
    for newstatement in set(result.get(newdate)):
    	if newstatement not in f.read():
	    f.write(newstatement + "\n")
	    f.flush()


#Reset the cursor to the beginning of the file


fhtml = open("LaPosteHTML.html","w")
############ HEADER ############
fhtml.write("""
<html>
    <head>
    <!-- script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4/jquery.min.js"></script>
    <script type="text/javascript" src="http://github.com/riklomas/quicksearch/raw/dev/jquery.quicksearch.js"></script-->

    <script type="text/javascript" src="./jquery.min.js"></script> 
    <script type="text/javascript" src="./jquery.quicksearch.js"></script> 
	<script type="text/javascript" src="./custom.js"></script> 
    <script type="text/javascript"> 
$(function () {
    $('input#id_search').quicksearch('table#account tbody tr');
});
    </script>
    </head>

<body>
<form action="#"> 
<fieldset> 
<input type="text" id="id_search" />
</fieldset> 
</form> 

""");
############# BODY ############
f.seek(0)
prevdate = 0
color = 0
prevmonth = 0
newmonth = False
fhtml.write("<table id=\"account\" border='1'><tr><th>Month</th><th>Date</th><th>Libelle</th><th>Montant (Euros)<br />%s</th><th>Montant(Francs)<br />%s</th></tr>" % (euros,francs))
for line in f.readlines():
    components = line.split(";")

    #Debut/Credit
    amountEuro = components[2] 
    amountFrancs = components[3]
    if amountEuro[0] == '-':
	components[2] = "<td style=\"color:red\" class=\"amountEuro minus\">%s</td>" % amountEuro
	components[3] = "<td style=\"color:red\" class=\"red\">%s</td>" % amountFrancs
    else:
	components[2] = "<td style=\"color:blue\" class=\"amountEuro plus\">%s</td>" % amountEuro
	components[3] = "<td style=\"color:blue\" class=\"blue\">%s</td>" % amountFrancs
	

    month = components[0].split("/")[1]

    #Check same date
    if	prevdate != components[0]:
    	prevdate = components[0]
	if color == "green":
	    color = "blue"
	else:
	    color = "green"
	
    components.insert(0,color)

    # Month
    if prevmonth != month:
	components.insert(1,"<td style=\"background-color:yellow\" class=\"month_name\">%s</td>" % mycalendar[month]) 
	prevmonth = month
	newmonth = True
    else:
	newmonth = False
	components.insert(1,'<td><span></span></td>') 
	
    if newmonth == True:
    	fhtml.write("<tr class=\"month\"style=\"color:%s\">%s<td>%s</td><td>%s</td>%s%s</tr>" % tuple(components))
    else:
    	fhtml.write("<tr style=\"color:%s\">%s<td>%s</td><td>%s</td>%s%s</tr>" % tuple(components))

    
fhtml.write("</table>")


############ FOOTER ############
fhtml.write("""
</body>
</html>
""");
fhtml.close()
f.close()



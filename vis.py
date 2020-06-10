import pandas as pd
import numpy as np
import math
import time
import os
import platform
import webbrowser
chrome_path = 'open -a /Applications/Google\ Chrome.app %s'
#InputLogFileName = "C:\\Users\\sidsc\\source\\repos\\Paxos\\plan\\nodelog1.csv"
InputLogFileName = "nodelog-1.csv"
OutputHtmlFileName = "Result.html"
def AddHtmlStart(content):
    result  = "<html>" +  content
    return result
def AddHtmlEnd(content):
    result  = content + "</html>"
    return result

def AddHeadStart(content):
    result  = "<head>" +  content
    return result
def AddHeadEnd(content):
    result  = content + "</head>"
    return result

def AddBodyStart(content):
    result  = "<body>" +  content
    return result
def AddBodyEnd(content):
    result  = content + "</body>"
    return result

def AddTableStart(content):
    result  = "<table style=\"width:100%\">" + content
    return result
def AddTableEnd(content):
    result  = content + "</table>"
    return result

def AddTableRowStart(content):
    result  = "<tr>" + content
    return result
def AddTableRowEnd(content):
    result  = content + "</tr>"
    return result
def AddTableCol(content):
    result  = "<td>" + content + "</td>"
    return result
def AddTableHeaderCol(content):
    result  = "<th>" + content + "</th>"
    return result

def boundHtmlTag(content):
    result = AddHtmlStart(content)
    result = AddHtmlEnd(result)
    return result

def boundHeadTag(content):
    result = AddHeadStart(content)
    result = AddHeadEnd(result)
    return result

def boundBodyTag(content):
    result = AddBodyStart(content)
    result = AddBodyEnd(result)
    return result

def boundTableTag(content):
    result = AddTableStart(content)
    result = AddTableEnd(result)
    return result

nextFileReadTimegap = 2 # seconds gap reading new file if there is no modification.
newRowShowGap = 0.750 # 750 ms

def creation_date(path_to_file):
    return os.path.getmtime(path_to_file)

# Generate a list of all node names Node0, Node1 ... and so on
def GetNodeList(df):
    nodelist = list()
    nodeids = df['nodeid'].unique()
    for x in nodeids:
      nodename = "Node: " + str(x) + '        '
      nodelist.append(nodename)

    nodecountreceiver = df['receiverid'].unique()
    for x in nodecountreceiver:
      nodename = "Node: " + str(x) + '        '
      if(nodename not in nodelist):
          nodelist.append(nodename)
    return nodelist
lastmodificationtime = 0
webbrowser.get(chrome_path).open("./visual.html")
while(1):
    if not os.path.isfile(InputLogFileName):
        time.sleep(nextFileReadTimegap) # Sleep for 3 seconds if file not exists.
        continue
    newmodificationtime = creation_date(InputLogFileName);
    if(lastmodificationtime == newmodificationtime):
        time.sleep(nextFileReadTimegap)
        continue
    lastmodificationtime = newmodificationtime
    # Keep reading the file at least every two seconds.
    time.sleep(1)
    logpath = "./nodelog-1.csv"
    logdata = pd.read_csv(logpath)
    
    countOfRowsProcessed = 0
    for (idx, row) in logdata.iterrows():
        if(idx >= countOfRowsProcessed): # only wait for next row which has not been shown before, using earlier file fragment.
            time.sleep(newRowShowGap)
        df = logdata.iloc[0:idx]
        
        # Generate a list of all node names Node0, Node1 ... and so on
        nodelist = GetNodeList(df)
        #print(nodelist)
        nodecount = len(nodelist)
        # Create header Row
        strTable = ""
        strTable += "<tr>" # start row
        for i in range(nodecount):
            strTable += "<th>" + nodelist[i] + '                ' +  "</th>"
        strTable += "</tr>" # end row
        
        
        # Now add rows for each log row
        for (idx, row) in df.iterrows():
            strTable += "<tr>" # start row

            # first collect all the variables
            timestamp = row.loc['timeStamp']
            nodeid = row.loc['nodeid']
            receiverid = row.loc['receiverid']
            nodeAlive = row.loc['nodeAlive']
            N = row.loc['N']
            value = row.loc['value']
            nodeRole = row.loc['nodeRole']
            maxPromisedN = row.loc['maxPromisedN']
            consensusValue = row.loc['consensusValue']
            currentAction = row.loc['currentAction']

            for i in range(nodecount):
                sendPacket = ""
                if(nodeAlive != 1):
                    strTable += "<td bgcolor=\"F75D59\">" + 'Crashed' + "</td>"
                elif(i == nodeid):
                    #strTable += "<td bgcolor=\"green\">" + 'Send' + "</td>"
                    sendPacket += str(N)
                    if(value != -1): # Only N
                        sendPacket += ", " + str(value)
                    if(currentAction == 1):
                        strTable += "<td bgcolor=\"FFFF00\">" + 'Prepare: ' + sendPacket + "</td>"
                    elif(currentAction == 2):
                        strTable += "<td bgcolor=\"EDDA74\">" + 'Prepare_Ack: ' + sendPacket + "</td>"
                    elif(currentAction == 3):
                        strTable += "<td bgcolor=\"82CAFF\">" + 'Propose: ' + sendPacket + "</td>"
                    elif(currentAction == 4):
                        strTable += "<td bgcolor=\"1589FF\">" + 'Propose_Ack: ' + sendPacket + "</td>"
                    elif(currentAction == 6):
                        strTable += "<td bgcolor=\"4E9258\">" + 'Consensus: ' + sendPacket + "</td>"
                elif(i == receiverid):
                    strTable += "<td>" + 'Receiver' + "</td>"
                else:
                    strTable += "<td>" +  ' ' + "</td>"
            strTable += "</tr>" # end row
        strTable = boundTableTag(strTable)
        
        # Now built full html

        # Add html head
        strStyle = "<style > table, th, td {   border: 1px solid black; }</style> "
        strStyle += "<script type=\"text/javascript\">     setTimeout(function () {  location.reload(); }, 500); </script>"
        strhead = boundHeadTag(strStyle)

        # Get html body
        strbody = boundBodyTag(strTable)
        strResult = strhead + strbody
        strResult = boundHtmlTag(strResult)
        hs = open("visual.html", 'w')
        hs.write(strResult)
        hs.close()
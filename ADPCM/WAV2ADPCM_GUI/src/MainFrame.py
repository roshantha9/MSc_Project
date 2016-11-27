###############################################################################
#	MainFrame.py
#	Contains 3 buttons inside a toolbar object, 2 list control objects and
#	splitters
#   
#   Author: Rosh Mendis (adapted from http://zetcode.com/wxpython/draganddrop/)
###############################################################################
import wx
import os
import subprocess
from time import gmtime, strftime

from pprint import pprint
from TextDropTarget import TextDropTarget

class MainFrame(wx.Frame):
    
    
    def __init__(self, parent, id, title):
        
        # ==== global vars ====
        self.FileList = [];   # list of files the user selected
        self.CurrentDir='';     
        
        # ==== draw the toolbar Buttons ====
        wx.Frame.__init__(self, parent, id, title, wx.DefaultPosition, wx.Size(900, 400))       
        toolbar = wx.ToolBar(self, 1, style=wx.TB_HORIZONTAL | wx.NO_BORDER)
        toolbar.AddControl(wx.Button( toolbar, 1, "Encode", (1, 1), wx.DefaultSize ) )
        wx.EVT_BUTTON( self, 1, self.MnbEncodeEvent )
        toolbar.AddControl(wx.Button( toolbar, 2, "Select-All", (80, 1), wx.DefaultSize ) )
        wx.EVT_BUTTON( self, 2, self.MnbSelectAllEvent )
        toolbar.AddControl(wx.Button( toolbar, 3, "Unselect-All", (160, 1), wx.DefaultSize ) )
        wx.EVT_BUTTON( self, 3, self.MnbUnSelectAllEvent )       
        self.SetToolBar(toolbar)
        
        # ==== draw the file lists, splitters etc ====
        splitter1 = wx.SplitterWindow(self, -1, style=wx.SP_3D)
        splitter2 = wx.SplitterWindow(splitter1, -1, style=wx.SP_3D)
        self.dir = wx.GenericDirCtrl(splitter1, -1, dir='D:/cygwin/home/DSE/adpcm', style=wx.DIRCTRL_DIR_ONLY)
        self.lc1 = wx.ListCtrl(splitter2, -1, style=wx.LC_LIST)
        self.lc2 = wx.ListCtrl(splitter2, -1, style=wx.LC_LIST)
        dt = TextDropTarget(self.lc2)
        self.lc2.SetDropTarget(dt)
        wx.EVT_LIST_BEGIN_DRAG(self, self.lc1.GetId(), self.OnDragInit)
        wx.EVT_LIST_ITEM_RIGHT_CLICK(self, self.lc2.GetId(), self.OnLC2RC)        
        tree = self.dir.GetTreeCtrl()
        splitter2.SplitHorizontally(self.lc1, self.lc2)
        splitter1.SplitVertically(self.dir, splitter2)
        wx.EVT_TREE_SEL_CHANGED(self, tree.GetId(), self.OnSelect)
        self.OnSelect(0)
        self.Centre()
              
    def OnLC2RC(self, event):
        #remove from logical list
        text = self.lc2.GetItemText(event.GetIndex())
        self.FileList.remove(text)
        # remove from graphical list
        self.lc2.DeleteItem(event.GetIndex())
        
    
    def OnSelect(self, event):        
        self.CurrentDir = self.dir.GetPath(); # update current dir
        list = os.listdir(self.dir.GetPath())
                
        self.lc1.ClearAll()
        self.lc2.ClearAll()
        for i in range(len(list)):
            if list[i][0] != '.':
                if '.wav' in list[i]:   # filter by only 'wav' files
                    self.lc1.InsertStringItem(0, list[i])

    def OnDragInit(self, event):
        text = self.lc1.GetItemText(event.GetIndex())
        tdo = wx.PyTextDataObject(text)
        
        # only add to the list if not already in list        
        if tdo.GetText() not in self.FileList:        
            tds = wx.DropSource(self.lc1)
            tds.SetData(tdo)
            tds.DoDragDrop(True)        
            self.FileList.append(tdo.GetText());    
  
        
    def MnbEncodeEvent(self, event):
        FileList_w_fullpath=[]
        
        # construct the file list (with full path)
        for item in self.FileList:
            FileList_w_fullpath.append(self.CurrentDir+'\\'+item) 
        
        # call the ADPCM Encoder for each file and save result in special folder
        OutputFolderName = 'ADPCM_OUTPUT'
        
        # if output folder does not exist create it
        if(os.path.exists(self.CurrentDir+'\\'+OutputFolderName)==False):
            os.mkdir(self.CurrentDir+'\\'+OutputFolderName)
        
        # --- write to log file ---
        LOGFILE = open("WAV2ADPCM.log","a")
        LOGFILE.write(" "); #empty line
        LOGFILE.write("************************ <Batch Started> *************************\r\n")
        LOGFILE.write(strftime("%Y-%m-%d %H:%M:%S", gmtime())+"\r\n")
        
        i=0
        for item in FileList_w_fullpath:
            ext_command = 'ADPCM_Enc.exe ' + "\""+item+"\" "+"\""+ self.CurrentDir + "\\" + OutputFolderName + "\\" + self.FileList[i].replace('wav','adp')+"\""
            process = subprocess.Popen(ext_command,shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)   
            #process.wait()              
            err_output = process.communicate()[1]
            std_output = process.communicate()[0]
            
            # --- write to log file ---             
            LOGFILE.write(ext_command+"\r\n")
            LOGFILE.write(std_output+"\r\n")
            LOGFILE.write(err_output+"\r\n")
            
            i=i+1; #increment counter to get next filename
            
        LOGFILE.write("************************ <!Batch Ended!> *************************\r\n")   
        LOGFILE.close() # close log file   
    
        #pprint(FileList_w_fullpath)
  
    
    def MnbSelectAllEvent(self, event):
        list = os.listdir(self.dir.GetPath())
        
        # reset the second list control
        self.FileList =[]
        self.lc2.ClearAll()        

        # populate the second list control with the items in the first one
        for idx in range(self.lc1.GetItemCount()):
            item = self.lc1.GetItem(idx)            
            self.lc2.InsertStringItem(0, item.GetText()) # add to physical list
            self.FileList.append(item.GetText()) # add to logical list       

    
    def MnbUnSelectAllEvent(self, event):
        # remove all logical items from list
        self.FileList =[];
         
        # remove all physical items
        self.lc2.ClearAll()
        self.lc2.DeleteAllItems()
        
        
class App(wx.App):  
    def OnInit(self):
        frame = MainFrame(None, -1, "WAV-to-ADPCM-Converter")
        frame.Show(True)
        self.SetTopWindow(frame)  
        return True
    
    
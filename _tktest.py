# test script for Tk
import Tkinter
from Tkconstants import *
tk = Tkinter.Tk()
frame = Tkinter.Frame(tk, relief=RIDGE, borderwidth=2)
frame.pack(fill=BOTH,expand=1)
label = Tkinter.Label(frame, text="Hello, World")
label.pack(fill=X, expand=1)
button = Tkinter.Button(frame,text="Exit",command=tk.destroy)
button.pack(side=BOTTOM)
tk.mainloop()

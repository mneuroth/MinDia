# set new show time for all dias
import mindiapy

mindiapy.PrintLn( "set new show time for all slides" )

aObj = mindiapy.GetDiaPresentation()
#s = "count=%d" % aObj.GetDiaCount()
#mindiapy.PrintLn( s )

for i in range( 0, aObj.GetDiaCount() ):
	aItem = aObj.GetDia( i )
	s = "dia %d id=%s dissolve=%f show=%f" % (i, aItem.GetId(), aItem.GetDissolveTime(), aItem.GetShowTime())
	aItem.SetShowTime( aItem.GetShowTime()*1.125 )
	#sId = "id%d" % (i+1)
	#aItem.SetId( sId )
	mindiapy.PrintLn( s )
 
aItem = aObj.GetDia( 0 )
# test dia
if aItem:
	mindiapy.PrintLn( "then" )
else:
	mindiapy.PrintLn( "else" )
s = "dia %d dissolve=%f show=%f" % (0, aItem.GetDissolveTime(), aItem.GetShowTime())
mindiapy.PrintLn( s )
#bla

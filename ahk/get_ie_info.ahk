IELoad(Pwb)
{
	If !Pwb
		Return 1
	Loop   ;Otherwise sleep for .1 seconds until the page starts loading
		Sleep,100
	Until (Pwb.busy)
	Loop   ;Once it starts loading wait until completes
		Sleep,100
	Until (!Pwb.busy)
	Loop   ;optional check to wait for the page to completely load
		Sleep,100
		Until (Pwb.Document.Readystate = "Complete")
	Return 0
}

ie := ComObjCreate("InternetExplorer.Application")
ie.Visible := true  ; This is known to work incorrectly on IE7.
ie.Navigate("http://www.google.com/")
IELoad(ie)
ie.Document.All.q.Value := "Coucou Jean-Louis!"
ie.Document.getElementsByName("q")[0].Value := "Jean-Louis GUENEGO"
inputs := ie.Document.getElementsByTagName("input")
count := inputs.length
Loop %count%
{
	value := inputs[ A_Index-1 ].getAttribute("name")
	MsgBox val= %value%
	if value = btnG
	{
		MsgBox about to click.
		inputs[ A_Index-1 ].click()
		Return
	}
}
MsgBox OK
;ie.quit
Return
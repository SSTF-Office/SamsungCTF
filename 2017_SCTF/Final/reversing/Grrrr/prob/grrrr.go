package __main__
import (
	πg "grumpy"
)
var Code *πg.Code
func init() {
	Code = πg.NewCode("<module>", "/home/zzoru/a.py", nil, 0, func(πF *πg.Frame, _ []*πg.Object) (*πg.Object, *πg.BaseException) {
		var πR *πg.Object; _ = πR
		var πE *πg.BaseException; _ = πE
		ß__main__ := πg.InternStr("__main__")
		ß__name__ := πg.InternStr("__name__")
		ßargv := πg.InternStr("argv")
		ßflag := πg.InternStr("flag")
		ßkey := πg.InternStr("key")
		ßlen := πg.InternStr("len")
		ßord := πg.InternStr("ord")
		ßos := πg.InternStr("os")
		ßsys := πg.InternStr("sys")
		var πTemp001 *πg.Object
		_ = πTemp001
		var πTemp002 []*πg.Object
		_ = πTemp002
		var πTemp003 *πg.Object
		_ = πTemp003
		var πTemp004 bool
		_ = πTemp004
		var πTemp005 *πg.Object
		_ = πTemp005
		var πTemp006 *πg.Object
		_ = πTemp006
		var πTemp007 *πg.Object
		_ = πTemp007
		for ; πF.State() >= 0; πF.PopCheckpoint() {
			switch πF.State() {
			case 0:
			default: panic("unexpected function state")
			}
			// line 1: import os
			πF.SetLineno(1)
			if πTemp002, πE = πg.ImportModule(πF, "os"); πE != nil {
				continue
			}
			πTemp001 = πTemp002[0]
			if πE = πF.Globals().SetItem(πF, ßos.ToObject(), πTemp001); πE != nil {
				continue
			}
			// line 2: import sys
			πF.SetLineno(2)
			if πTemp002, πE = πg.ImportModule(πF, "sys"); πE != nil {
				continue
			}
			πTemp001 = πTemp002[0]
			if πE = πF.Globals().SetItem(πF, ßsys.ToObject(), πTemp001); πE != nil {
				continue
			}
			// line 4: key = "Grumpy makes me suck!"
			πF.SetLineno(4)
			if πE = πF.Globals().SetItem(πF, ßkey.ToObject(), πg.NewStr("Grumpy makes me suck!").ToObject()); πE != nil {
				continue
			}
			if πTemp003, πE = πg.ResolveGlobal(πF, ß__name__); πE != nil {
				continue
			}
			if πTemp001, πE = πg.Eq(πF, πTemp003, ß__main__.ToObject()); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label1
			}
			goto Label2
			// line 5: if __name__ == "__main__":
			πF.SetLineno(5)
		Label1:
			πTemp002 = πF.MakeArgs(1)
			if πTemp003, πE = πg.ResolveGlobal(πF, ßsys); πE != nil {
				continue
			}
			if πTemp005, πE = πg.GetAttr(πF, πTemp003, ßargv, nil); πE != nil {
				continue
			}
			πTemp002[0] = πTemp005
			if πTemp003, πE = πg.ResolveGlobal(πF, ßlen); πE != nil {
				continue
			}
			if πTemp005, πE = πTemp003.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.NE(πF, πTemp005, πg.NewInt(2).ToObject()); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label3
			}
			goto Label4
			// line 6: if len(sys.argv) != 2:
			πF.SetLineno(6)
		Label3:
			// line 7: print 'Usage grrrr.py [flag]'
			πF.SetLineno(7)
			πTemp002 = make([]*πg.Object, 1)
			πTemp002[0] = πg.NewStr("Usage grrrr.py [flag]").ToObject()
			if πE = πg.Print(πF, πTemp002, true); πE != nil {
				continue
			}
			goto Label5
		Label4:
			// line 9: flag = sys.argv[1]
			πF.SetLineno(9)
			πTemp001 = πg.NewInt(1).ToObject()
			if πTemp005, πE = πg.ResolveGlobal(πF, ßsys); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetAttr(πF, πTemp005, ßargv, nil); πE != nil {
				continue
			}
			if πTemp003, πE = πg.GetItem(πF, πTemp006, πTemp001); πE != nil {
				continue
			}
			if πE = πF.Globals().SetItem(πF, ßflag.ToObject(), πTemp003); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			if πTemp003, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			πTemp002[0] = πTemp003
			if πTemp003, πE = πg.ResolveGlobal(πF, ßlen); πE != nil {
				continue
			}
			if πTemp005, πE = πTemp003.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp005, πg.NewInt(27).ToObject()); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label6
			}
			goto Label7
			// line 10: if len(flag) == 27:
			πF.SetLineno(10)
		Label6:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(0).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(20).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(0).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label8
			}
			goto Label9
			// line 11: if ord(flag[0]) ^ 20 == ord(key[0]):
			πF.SetLineno(11)
		Label8:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(1).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(49).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(1).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label10
			}
			goto Label11
			// line 12: if ord(flag[1]) ^ 49 == ord(key[1]):
			πF.SetLineno(12)
		Label10:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(2).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(33).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(2).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label12
			}
			goto Label13
			// line 13: if ord(flag[2]) ^ 33 == ord(key[2]):
			πF.SetLineno(13)
		Label12:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(3).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(43).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(3).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label14
			}
			goto Label15
			// line 14: if ord(flag[3]) ^ 43 == ord(key[3]):
			πF.SetLineno(14)
		Label14:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(4).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(11).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(4).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label16
			}
			goto Label17
			// line 15: if ord(flag[4]) ^ 11 == ord(key[4]):
			πF.SetLineno(15)
		Label16:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(5).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(79).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(5).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label18
			}
			goto Label19
			// line 16: if ord(flag[5]) ^ 79 == ord(key[5]):
			πF.SetLineno(16)
		Label18:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(6).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(82).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(6).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label20
			}
			goto Label21
			// line 17: if ord(flag[6]) ^ 82 == ord(key[6]):
			πF.SetLineno(17)
		Label20:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(7).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(24).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(7).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label22
			}
			goto Label23
			// line 18: if ord(flag[7]) ^ 24 == ord(key[7]):
			πF.SetLineno(18)
		Label22:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(8).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(12).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(8).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label24
			}
			goto Label25
			// line 19: if ord(flag[8]) ^ 12 == ord(key[8]):
			πF.SetLineno(19)
		Label24:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(9).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(27).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(9).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label26
			}
			goto Label27
			// line 20: if ord(flag[9]) ^ 27 == ord(key[9]):
			πF.SetLineno(20)
		Label26:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(10).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(28).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(10).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label28
			}
			goto Label29
			// line 21: if ord(flag[10]) ^ 28 == ord(key[10]):
			πF.SetLineno(21)
		Label28:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(11).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(44).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(11).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label30
			}
			goto Label31
			// line 22: if ord(flag[11]) ^ 44 == ord(key[11]):
			πF.SetLineno(22)
		Label30:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(12).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(67).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(12).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label32
			}
			goto Label33
			// line 23: if ord(flag[12]) ^ 67 == ord(key[12]):
			πF.SetLineno(23)
		Label32:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(13).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(89).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(13).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label34
			}
			goto Label35
			// line 24: if ord(flag[13]) ^ 89 == ord(key[13]):
			πF.SetLineno(24)
		Label34:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(14).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(82).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(14).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label36
			}
			goto Label37
			// line 25: if ord(flag[14]) ^ 82 == ord(key[14]):
			πF.SetLineno(25)
		Label36:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(15).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(127).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(15).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label38
			}
			goto Label39
			// line 26: if ord(flag[15]) ^ 127 == ord(key[15]):
			πF.SetLineno(26)
		Label38:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(16).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(30).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(16).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label40
			}
			goto Label41
			// line 27: if ord(flag[16]) ^ 30 == ord(key[16]):
			πF.SetLineno(27)
		Label40:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(17).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(70).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(17).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label42
			}
			goto Label43
			// line 28: if ord(flag[17]) ^ 70 == ord(key[17]):
			πF.SetLineno(28)
		Label42:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(18).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(83).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(18).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label44
			}
			goto Label45
			// line 29: if ord(flag[18]) ^ 83 == ord(key[18]):
			πF.SetLineno(29)
		Label44:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(19).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(28).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(19).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label46
			}
			goto Label47
			// line 30: if ord(flag[19]) ^ 28 == ord(key[19]):
			πF.SetLineno(30)
		Label46:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(20).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(20).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(20).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label48
			}
			goto Label49
			// line 31: if ord(flag[20]) ^ 20 == ord(key[20]):
			πF.SetLineno(31)
		Label48:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(21).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(24).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(0).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label50
			}
			goto Label51
			// line 32: if ord(flag[21]) ^ 24 == ord(key[0]):
			πF.SetLineno(32)
		Label50:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(22).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(68).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(1).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label52
			}
			goto Label53
			// line 33: if ord(flag[22]) ^ 68 == ord(key[1]):
			πF.SetLineno(33)
		Label52:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(23).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(7).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(2).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label54
			}
			goto Label55
			// line 34: if ord(flag[23]) ^ 7 == ord(key[2]):
			πF.SetLineno(34)
		Label54:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(24).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(31).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(3).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label56
			}
			goto Label57
			// line 35: if ord(flag[24]) ^ 31 == ord(key[3]):
			πF.SetLineno(35)
		Label56:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(25).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(2).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(4).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label58
			}
			goto Label59
			// line 36: if ord(flag[25]) ^ 2 == ord(key[4]):
			πF.SetLineno(36)
		Label58:
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(26).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßflag); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp003, πE = πg.Xor(πF, πTemp006, πg.NewInt(4).ToObject()); πE != nil {
				continue
			}
			πTemp002 = πF.MakeArgs(1)
			πTemp005 = πg.NewInt(5).ToObject()
			if πTemp007, πE = πg.ResolveGlobal(πF, ßkey); πE != nil {
				continue
			}
			if πTemp006, πE = πg.GetItem(πF, πTemp007, πTemp005); πE != nil {
				continue
			}
			πTemp002[0] = πTemp006
			if πTemp005, πE = πg.ResolveGlobal(πF, ßord); πE != nil {
				continue
			}
			if πTemp006, πE = πTemp005.Call(πF, πTemp002, nil); πE != nil {
				continue
			}
			πF.FreeArgs(πTemp002)
			if πTemp001, πE = πg.Eq(πF, πTemp003, πTemp006); πE != nil {
				continue
			}
			if πTemp004, πE = πg.IsTrue(πF, πTemp001); πE != nil {
				continue
			}
			if πTemp004 {
				goto Label60
			}
			goto Label61
			// line 37: if ord(flag[26]) ^ 4 == ord(key[5]):
			πF.SetLineno(37)
		Label60:
			// line 38: print "Congratulations!"
			πF.SetLineno(38)
			πTemp002 = make([]*πg.Object, 1)
			πTemp002[0] = πg.NewStr("Congratulations!").ToObject()
			if πE = πg.Print(πF, πTemp002, true); πE != nil {
				continue
			}
			goto Label61
		Label61:
			goto Label59
		Label59:
			goto Label57
		Label57:
			goto Label55
		Label55:
			goto Label53
		Label53:
			goto Label51
		Label51:
			goto Label49
		Label49:
			goto Label47
		Label47:
			goto Label45
		Label45:
			goto Label43
		Label43:
			goto Label41
		Label41:
			goto Label39
		Label39:
			goto Label37
		Label37:
			goto Label35
		Label35:
			goto Label33
		Label33:
			goto Label31
		Label31:
			goto Label29
		Label29:
			goto Label27
		Label27:
			goto Label25
		Label25:
			goto Label23
		Label23:
			goto Label21
		Label21:
			goto Label19
		Label19:
			goto Label17
		Label17:
			goto Label15
		Label15:
			goto Label13
		Label13:
			goto Label11
		Label11:
			goto Label9
		Label9:
			goto Label7
		Label7:
			goto Label5
		Label5:
			goto Label2
		Label2:
		}
		return nil, πE
	})
	πg.RegisterModule("__main__", Code)
}

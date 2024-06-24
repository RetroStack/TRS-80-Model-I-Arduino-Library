/*

* It seems with Arduino, setting a PIN to INPUT doesn't always make the signal
  it was when it was an OUTPUT go 'neutral' as the M1 signals are mostly
  active LOW. It seems best to force such signal to HIGH before setting to
  INPUTS. This most critical when reading as last operation could have been
  a write. This may also have to do with the M1 test mode leaving the signals
  floating. 

* Not all objects are classes, will do so once more methods are added to various objects (i.e. keyboard)

*/

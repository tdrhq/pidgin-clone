;NSIS Modern User Interface - Language File
;Compatible with Modern UI 1.68

;Language: Slovenian (1060)
;By Janez Dolinar

;--------------------------------

!insertmacro MUI_LANGUAGEFILE_BEGIN "Slovenian"

  !define MUI_LANGNAME "Slovenski jezik" ;Use only ASCII characters (if this is not possible, use the English name)

  !define MUI_TEXT_WELCOME_INFO_TITLE "Dobrodo�li v �arovniku $(^NameDA)"
  !define MUI_TEXT_WELCOME_INFO_TEXT "Ta �arovnik vam bo pomagal pri namestitvi programa $(^NameDA).\r\n\r\nPriporo�amo vam, da zaprete vsa ostala okna in programe pred namestitvijo. To bo omogo�ilo nemoteno namestitev programa in njegovih delov brez ponovnega zagona ra�unalnika.\r\n\r\n$_CLICK"
  
  !define MUI_TEXT_LICENSE_TITLE "Licen�na pogodba"  
  !define MUI_TEXT_LICENSE_SUBTITLE "Prosimo, preglejte pogoje pogodbe pred namestitvijo $(^NameDA)."
  !define MUI_INNERTEXT_LICENSE_TOP "Za preostali del pogodbe pritisnite tipko 'Page Down'."
  !define MUI_INNERTEXT_LICENSE_BOTTOM "�e se strinjate s pogoji, pritisnite Se strinjam. Da bi lahko namestili $(^NameDA), se morate s pogodbo strinjati."
  !define MUI_INNERTEXT_LICENSE_BOTTOM_CHECKBOX "�e se strinjate z licen�nimi pogoji pogodbe, spodaj obkljukajte ustrezno okence. Za namestitev $(^NameDA) se morate strinjati s pogoji pogodbe. $_CLICK"
  !define MUI_INNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS "�e se strinjate z licen�nimi pogoji pogodbe, spodaj izberite prvo mo�nost. Za namestitev $(^NameDA) se morate strinjati s pogoji pogodbe. $_CLICK"
  
  !define MUI_TEXT_COMPONENTS_TITLE "Izberite bloke"
  !define MUI_TEXT_COMPONENTS_SUBTITLE "Izberite si, katere bloke izdelka $(^NameDA) �elite namestiti."
  !define MUI_INNERTEXT_COMPONENTS_DESCRIPTION_TITLE "Opis"
  !define MUI_INNERTEXT_COMPONENTS_DESCRIPTION_INFO "Povlecite mi�ko nad blok, da vidite njegov opis."
  
  !define MUI_TEXT_DIRECTORY_TITLE "Izberite si pot namestive"
  !define MUI_TEXT_DIRECTORY_SUBTITLE "Izberite si mapo, v katero boste $(^NameDA) namestili."
  
  !define MUI_TEXT_INSTALLING_TITLE "Name��anje poteka"
  !define MUI_TEXT_INSTALLING_SUBTITLE "Prosimo po�akajte, $(^NameDA) se name��a."
  
  !define MUI_TEXT_FINISH_TITLE "Namestitev dokon�ana"
  !define MUI_TEXT_FINISH_SUBTITLE "Namestitev je bila kon�ana uspe�no."
  
  !define MUI_TEXT_ABORT_TITLE "Namestitev je bila prekinjena"
  !define MUI_TEXT_ABORT_SUBTITLE "Namestitev ni bila kon�ana uspe�no."
  
  !define MUI_BUTTONTEXT_FINISH "&Kon�aj"
  !define MUI_TEXT_FINISH_INFO_TITLE "Zaklju�evanje namestitve $(^NameDA)"
  !define MUI_TEXT_FINISH_INFO_TEXT "Paket $(^NameDA) je bil name��en na va� ra�unalnik.\r\n\r\nPritisnite Kon�aj za zaklju�ek programa."
  !define MUI_TEXT_FINISH_INFO_REBOOT "Za dokon�anje namestitve $(^NameDA) morate  ponovno zagnati ra�unalnik. �elite zdaj  ponovno zagnati ra�unalnik?"
  !define MUI_TEXT_FINISH_REBOOTNOW "Ponovni zagon"
  !define MUI_TEXT_FINISH_REBOOTLATER "Ra�unalnik �elim ponovno zagnati kasneje"
  !define MUI_TEXT_FINISH_RUN "Za�eni $(^NameDA)"
  !define MUI_TEXT_FINISH_SHOWREADME "Prika�i informacije"
  
  !define MUI_TEXT_STARTMENU_TITLE "Izberite mapo menija Start"
  !define MUI_TEXT_STARTMENU_SUBTITLE "Izberite mapo menija Start, kjer bodo bli�njice do programa."
  !define MUI_INNERTEXT_STARTMENU_TOP "Izberite mapo menija Start, kjer bi �eleli ustvariti bli�njico do programa. �e vpi�ete poljubno ime, se bo ustvarila mapa s tem imenom."
  !define MUI_INNERTEXT_STARTMENU_CHECKBOX "Ne ustvari bli�njice"
  
  !define MUI_TEXT_ABORTWARNING "Ste prepri�ani, da �elite prekiniti namestitev $(^Name)?"  
  
  
  !define MUI_UNTEXT_WELCOME_INFO_TITLE "Dobrodo�li v �arovniku za odstranitev $(^NameDA)"
  !define MUI_UNTEXT_WELCOME_INFO_TEXT "Ta �arovnik vas bo vodil skozi odstranitev $(^NameDA).\r\n\r\nPreden pri�nete z odstranitvijo, se prepri�ajte, da $(^NameDA) ni zagnan.\r\n\r\n$_CLICK"
  
  !define MUI_UNTEXT_CONFIRM_TITLE "Odstrani $(^NameDA)"
  !define MUI_UNTEXT_CONFIRM_SUBTITLE "Odstrani $(^NameDA) iz va�ega ra�unalnika."
  
  !define MUI_UNTEXT_LICENSE_TITLE "Licen�na pogodba"  
  !define MUI_UNTEXT_LICENSE_SUBTITLE "Prosimo vas, da pregledate pogoje licen�ne pogodbe pred odstranitvijo $(^NameDA)."
  !define MUI_UNINNERTEXT_LICENSE_BOTTOM "�e se strinjate s pogoji licen�ne pogodbe, kliknite na Se strinjam. Za odstranitev $(^NameDA) se morate strinjati s pogoji."
  !define MUI_UNINNERTEXT_LICENSE_BOTTOM_CHECKBOX "�e se strinjate s pogoji licen�ne pogodbe, kliknite na okence spodaj. Za odstranitev $(^NameDA) se morate strinjati s pogoji. $_CLICK"
  !define MUI_UNINNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS "�e se strinjate s pogoji licen�ne pogodbe, spodaj izberite prvo podano mo�nost. Za odstranitev $(^NameDA) se morate strinjati s pogoji. $_CLICK"

  !define MUI_UNTEXT_COMPONENTS_TITLE "Izberite bloke"
  !define MUI_UNTEXT_COMPONENTS_SUBTITLE "Izberite si bloke $(^NameDA), ki jih �elite odstraniti."
  
  !define MUI_UNTEXT_DIRECTORY_TITLE "Izberite mapo"
  !define MUI_UNTEXT_DIRECTORY_SUBTITLE "Izberite mapo, iz katere �elite odstraniti $(^NameDA)."
  
  !define MUI_UNTEXT_UNINSTALLING_TITLE "Odstranjevanje poteka"
  !define MUI_UNTEXT_UNINSTALLING_SUBTITLE "Prosimo po�akajte, dokler se paket $(^NameDA) odstranjuje."
    
  !define MUI_UNTEXT_FINISH_TITLE "Uninstallation Complete"
  !define MUI_UNTEXT_FINISH_SUBTITLE "Odstranitev je uspe�no kon�ana."

  !define MUI_UNTEXT_ABORT_TITLE "Odstranitev je bila prekinjena"
  !define MUI_UNTEXT_ABORT_SUBTITLE "Odstranitev ni bila kon�ana uspe�no."

  !define MUI_UNTEXT_FINISH_INFO_TITLE "�arovnik za odstranitev $(^NameDA) se zaklju�uje"
  !define MUI_UNTEXT_FINISH_INFO_TEXT "$(^NameDA) je bil odstranjen iz va�ega ra�unalnika.\r\n\r\nKliknite na Dokon�aj, da kon�ate delo s �arovnikom."
  !define MUI_UNTEXT_FINISH_INFO_REBOOT "Da bi se $(^NameDA) namestitev dokon�ala, morate ponovno zagnati ra�unalnik. �elite sedaj ponovno zagnati va� ra�unalnik?"
  
  !define MUI_UNTEXT_ABORTWARNING "Ste prepri�ani, da �elite zapustiti odstranitev $(^Name)?"  

!insertmacro MUI_LANGUAGEFILE_END
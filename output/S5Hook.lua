--[[   //  S5Hook  //  by yoq  // v0.10

    S5Hook.AddArchive(string path [, bool precedence])			Add a bba/s5x archive to the internal filesystem
                                                                 - if precedence is true all files will be loaded from it if they are inside
                                                                 
    S5Hook.Log(string textToLog)								Writes the string textToLog into the Settlers5 logfile
                                                                 - In MyDocuments/DIE SIEDLER - DEdK/Temp/Logs/Game/XXXX.log
    
    S5Hook.ChangeString(string identifier, string newString)	Changes the string with the given identifier to newString
                                                                 - ex: S5Hook.ChangeString("names/pu_serf", "Minion")  --change pu_serf from names.xml

    S5Hook.ReloadCutscenes()									Reload the cutscenes in a usermap after a savegame load.
                                                                 - call AFTER AddArchive()
    
    S5Hook.LoadGUI(string pathToXML)							Load a GUI definition from a .xml file.
                                                                 - call after AddArchive() for files inside the s5x archive
                                                                 - Completely replaces the old GUI --> Make sure all callbacks exist in the Lua script
                                                                 - Do NOT call this function in a GUI callback (button, chatinput, etc...)
                                                                 
    S5Hook.Eval(string luaCode)									Parses luaCode and returns a function, can be used to build a internal debugger
                                                                 - ex: myFunc = S5Hook.Eval("Message('Hello world')")
                                                                       myFunc()
                                                                       
    S5Hook.ReloadEntities()										Reloads all entity definitions, not the entities list -> only modifications are possible
                                                                 - In general: DO NOT USE, this can easily crash the game and requires extensive testing to get it right
                                                                 - Requires the map to be added with precedence
                                                                 - Only affects new entities -> reload map / reload savegame
                                                                 - To keep savegames working, it is only possible to make entities more complex (behaviour, props..)
                                                                   do not try to remove props/behaviours (ex: remove darios hawk), this breaks simple savegame loading
    
    S5Hook.SetSettlerMotivation(eID, motivation)				Set the motivation for a single settler (and only settlers, crashes otherwise ;)
                                                                 - motivation 1 = 100%, 0.2 = 20% settlers leaves
                                                                 
    S5Hook.GetWidgetPosition(widget)							Gets the widget position relative to its parent
                                                                - return1: X
                                                                - return2: Y
                                                                
    S5Hook.GetWidgetSize(widget)								Gets the size of the widget
                                                                - return1: width
                                                                - return2: height
                                                                
    S5Hook.IsEffectValid(effectID)								Checks whether this effectID is a valid effect, returns a bool

    S5Hook.CreateProjectile(									Creates a projectile effect, returns an effectID, which can be used with Logic.DestroyEffect()
                            int effectType,			-- from the GGL_Effects table
                            float startX, 
                            float startY, 
                            float targetX, 
                            float targetY 
                            int damage = 0,			-- optional, neccessary to do damage
                            float radius = -1,		-- optional, neccessary for area hit
                            int targetId = 0,		-- optional, neccessary for single hit
                            int attackerId = 0,		-- optional, used for events & allies when doing area hits
                            fn hitCallback)         -- optional, fires once the projectile reaches the target, return true to cancel damage events
                            
                                                                Single-Hit Projectiles:
                                                                    FXArrow, FXCrossBowArrow, FXCavalryArrow, FXCrossBowCavalryArrow, FXBulletRifleman, FXYukiShuriken, FXKalaArrow

                                                                Area-Hit Projectiles:
                                                                    FXCannonBall, FXCannonTowerBall, FXBalistaTowerArrow, FXCannonBallShrapnel, FXShotRifleman
                            

      
	MusicFix: allows Music.Start() to use the internal file system
			S5Hook.PatchMusicFix()										Activate
			S5Hook.UnpatchMusicFix()									Deactivate
																		 - ex: crickets as background music on full volume in an endless loop
																			   S5Hook.PatchMusicFix()
																			   Music.Start("sounds/ambientsounds/crickets_rnd_1.wav", 127, true)
																			 
							
	RuntimeStore: key/value store for strings across maps 
			S5Hook.RuntimeStore(string key, string value)				 - ex: S5Hook.RuntimeStore("addedS5X", "yes")
			S5Hook.RuntimeLoad(string key)								 - ex: if S5Hook.RuntimeLoad("addedS5X") ~= "yes" then [...] end
							
	CustomNames: individual names for entities
			S5Hook.SetCustomNames(table nameMapping)					Activates the function
			S5Hook.RemoveCustomNames()									Stop displaying the names from the table
																		 - ex: cnTable = { ["dario"] = "Darios new Name", ["erec"] = "Erecs new Name" }
																			   S5Hook.SetCustomNames(cnTable)
																			   cnTable["thief1"] = "Pete"		-- works since cnTable is a reference
	KeyTrigger: Callback for ALL keys with KeyUp / KeyDown
			S5Hook.SetKeyTrigger(func callbackFn)						Sets callbackFn as the callback for key events
			S5Hook.RemoveKeyTrigger()									Stop delivering events
																		 - ex: S5Hook.SetKeyTrigger(function (keyCode, keyIsUp)
																					Message(keyCode .. " is up: " .. tostring(keyIsUp))
																			   end)

	CharTrigger: Callback for pressed characters on keyboard
			S5Hook.SetCharTrigger(func callbackFn)						Sets callbackFn as the callback for char events
			S5Hook.RemoveCharTrigger()									Stop delivering events
																		 - ex: S5Hook.SetCharTrigger(function (charAsNum)
																					Message("Pressed: " .. string.char(charAsNum))
																			   end)
																			   
	OnScreenInformation (OSI): 
		Draw additional info near entities into the 3D-View (like healthbar, etc).
		You have to set a trigger function, which will be responsible for drawing 
		all info EVERY frame, so try to write efficient code ;)
		
			S5Hook.OSILoadImage(string path)							Loads a image and returns an image object
																		 - Images have to be reloaded after a savegame load
																		 - ex: imgObj = S5Hook.OSILoadImage("graphics\\textures\\gui\\onscreen_emotion_good")

			S5Hook.OSIGetImageSize(imgObj)								Returns sizeX and sizeY of the given image
																		 - ex: sizeX, sizeY = S5Hook.OSIGetImageSize(imgObj)

			S5Hook.OSISetDrawTrigger(func callbackFn)					callbackFn(eID, bool active, posX, posY) will be called EVERY frame for every 
																		   currently visible entity with overhead display, the active parameter become true
																		   
			S5Hook.OSIRemoveDrawTrigger()								Stop delivering events

		Only call from the DrawTrigger callback:
			S5Hook.OSIDrawImage(imgObj, posX, posY, sizeX, sizeY)		Draw the image on the screen. Stretching is allowed.
			
			S5Hook.OSIDrawText(text, font, posX, posY, r, g, b, a)		Draw the string on the screen. Valid values for font range from 1-10.
																		The color is specified by the r,g,b,a values (0-255).
																		a = 255 is maximum visibility
																		Standard S5 modifiers are allowed inside text (@center, etc...)
		Example:
		function SetupOSI()
			myImg = S5Hook.OSILoadImage("graphics\\textures\\gui\\onscreen_emotion_good")
			myImgW, myImgH = S5Hook.OSIGetImageSize(myImg)
			S5Hook.OSISetDrawTrigger(cbFn)
		end

		function cbFn(eID, active, x, y)
			if active then
				S5Hook.OSIDrawImage(myImg, x-myImgW/2, y-myImgH/2 - 40, myImgW, myImgH)
			else
				S5Hook.OSIDrawText("eID: " .. eID, 3, x+25, y, 255, 255, 128, 255)
			end
		end														
	
	Set up with InstallHook(_cb), this needs to be called again after loading a savegame.
	S5Hook becomes available after a few ticks!
	S5Hook only works with the newest patch version of Settlers5, 1.06!
]]

function InstallHook(installedCallback)
    if nil == string.find(Framework.GetProgramVersion(), "1.06.0217") then
        Message("Error: S5Hook requires version patch 1.06!")
        return
    end
    
    if not BigNum then
        Message("Error: S5Hook requires the BigNum library!")
        return
    end
    
    hook_icb = installedCallback
    hook_eID = Logic.CreateEntity(Entities.CU_Sheep, 1, 1, 0, 1)
    hook_loaded = false
    StartSimpleHiResJob("InstallHookNextTick")
end

function InstallHookNextTick()
    if hook_loaded then return true; end
    local sv67 = Logic.GetEntityScriptingValue(hook_eID, 67)
    if sv67 < 10000 then return; end
    
    local stage1 = { 3821797831, 6946934, 3050050024, 3757351423, 3229941921, 2926063733, 1778426330, 2952816704, 6815844, 4278206480, 1980782613, 1958774016, 2216034106, 2734716434, 10608624, 3757311431, 65697, 40501248, 3423993683, 2197845522, 822020292, 2198106367, 1750075584, 10616832, 3124988904, 214205439, 3268476760, 3435921412 }
    local cc = 'iaCkcAodMAAfddfeigpgpglAcfhdKAmaMkcAGechcgfgbglApcCkcAOfagbhegdgienhfhdgjgdeggjhiAmeCkcAQffgohagbhegdgienhfhdgjgdeggjhiAniDkcANepfdejemgpgbgeejgngbghgfAciEkcAQepfdejehgfheejgngbghgffdgjhkgfAfoEkcANepfdejeehcgbhhejgngbghgfAlfEkcAMepfdejeehcgbhhfegfhiheARFkcASepfdejfdgfheeehcgbhhfehcgjghghgfhcAdhFkcAVepfdejfcgfgngphggfeehcgbhhfehcgjghghgfhcAnoFkcANfchfgohegjgngffdhegphcgfAbkGkcAMfchfgohegjgngfemgpgbgeAfhGkcANedgigbgoghgffdhehcgjgoghAimGkcAEemgpghAkjGkcALebgegeebhcgdgigjhggfApiGkcAQfcgfgmgpgbgeedhfhehdgdgfgogfhdAbpHkcAIemgpgbgeehffejAdpHkcAFefhggbgmAggHkcAPfdgfheedhfhdhegpgneogbgngfhdAjaHkcASfcgfgngphggfedhfhdhegpgneogbgngfhdAfoIkcAPfdgfheedgigbhcfehcgjghghgfhcAcoIkcASfcgfgngphggfedgigbhcfehcgjghghgfhcApkIkcAOfdgfheelgfhjfehcgjghghgfhcAmkIkcARfcgfgngphggfelgfhjfehcgjghghgfhcAlcJkcAUfdgfheengphfhdgfeegphhgofehcgjghghgfhcAhiJkcAXfcgfgngphggfengphfhdgfeegphhgofehcgjghghgfhcAclKkcAVfdgfhefdgfhehegmgfhcengphegjhggbhegjgpgoAhaKkcAPfcgfgmgpgbgeefgohegjhegjgfhdAlcKkcASehgfhefhgjgeghgfhefagphdgjhegjgpgoAmnKkcAOehgfhefhgjgeghgfhefdgjhkgfAoiKkcARedhcgfgbhegffahcgpgkgfgdhegjgmgfAgdMkcAOejhdfggbgmgjgeefgggggfgdheAAAAAAAAAAAAAAAAAAAAAAAAAgaloLAkcAgkAppdgidmgFfgPlgegppBmggiAAkcAfdoiimkplhppiddoAhfoclibpkkeaAmgAojmheaBgngcgbAlihgkkeaAmgAojmheaBcogcgbAgbmdkbgiCkcAifmahecckdkeUhgAmhFgiCkcAAAAAmhFjoggejAilpaifpgggmhFkcggejAhehgdbmamdiddngiCkcAAhfchkbkeUhgAkdgiCkcAmhFkeUhgAllDkcAlijoggejAmgAojeamhAicjmfiAmgeaEjadbmamdijmgifpgPifhfgdkhppilheceIgaibomcmBAAijofilNiipaiiAilBffinfnEfdfgppfaUifmahefofgoifmlhjoppflfapphfApphfEgiAnghgAgiABAAinhfIfgoieghjlcppidmeYgkAfgkbMjpifApphaMppViiUhgAijmgifpghebmpphfEgkCfgppVUVhgAibmecmBAAijheceEgbojpmgckhppfaoidooglcppfiibmecmBAAgbojfpgdkhppgkCppheceIppVcaVhgAifmaheHfaoibloglcppfippcfgiCkcAloppAAAfgfgfgfgidomQnjoinjfmceMnjoinjfmceInjoonjfmceEnjoonjbmcegkBfdppVmmShgAidmeIfagkcioihcdnlkppfjijmboigbkfldppfafdppVmaShgAidmeIijnodbmaeamdidomIijofgkBfdppVnaShgAidmeIijmbffidmfEffoiNkgldppnjefpmnjefAoifaGAAoielGAAidmeIliCAAAmdidomQijofgkCfdppVcaShgAgkDfdppVcaShgAgkEfdppVcaShgAgkFfdppVcaShgAnjfnMnjfnInjfnEnjfnAffgkAgkBfdppVnaShgAidmeIfaoildgkldppijmboiligmldppidmedadbmamdidomQijofgkCfdppVcaShgAppeeceEidhmceEJhfopidmeInlfnMnlfnInlfnEnlfnAgkAgkAffgkAfanjbmcefanjbmcegkAfanlbmcegkBfdppVmmShgAidmeIfaoifhgkldppijmboinghcldppidmeQdbmamdgipanippppfdppVdeShgAkdgmCkcAidmeIlihlWfeAmgAojeamhApbooenAdbmamdkbgmCkcAifmahecofagipanippppfdppVdmShgAidmeMmhFhlWfeAffilomfgmhFhpWfeAfhilhnMmhFgmCkcAAAAAdbmamdffijoffgfhgailbnjmdkifAppdfgmCkcAgipanippppfdppVdiShgAilefMnleaeeoiGFAAdbmadiifHCAAPjfmafafdppVkiShgAilefInjeaEnjAoiogEAAoiobEAAgkAgkAgkEfdppVmiShgAgkAfdppVlaShgAidmecmgbojkcQlcppfgildfpanpkbAgkBfdppVmmShgAfafgppVfmShgAgkCfdppVmmShgAfafgppVfmShgAgipanippppfgppVfeShgAidmecifodbmamdfgildfpanpkbAgkBfdppVmmShgAfafgppVfmShgAgipanippppfgppVliShgAgkppfgppVmmShgAfafdppVfmShgAidmecifodbmaeamdgkCfdppVmmShgAidmeIfaoibkidllppfjfagkBfdppVmmShgAidmeIfaoilbggldppfkfkileeceomileaYijUiidbmamdgkBfdppVmmShgAidmeIfagiHAkcAoimfhllcppidmeIdbmamdgagkBfdppVmmShgAgkBfaoiogfhlcppgkCfdppVkmShgAidmeYifmaheFoiEAAAgbdbmamdildfiipaiiAineoEoinlljljppijmhepilegIppdeliilemlipmijMliephfpgipAmdgkBfdppVmmShgAidmeIifmaheDfaolFgipmjphhAkbemdekaAilIilBppfaMdbmamdgagkAgkBfdppVmmShgAidmeIfaoijifmldppijmboicjfnldppgbdbmamdgkAgkBfdppVmmShgAidmeIfafaoiemWlkppijeeceEfdppVkeShgAidmeQdbmaeamdliRpjfdAmgAojmheaBldOeoAmgeaonolgipanippppfdppVdeShgAkdhaCkcAidmeIdbmamdkbhaCkcAifmahecnfagipanippppfdppVdmShgAidmeMliRpjfdAmgAoimheaBGpfpoppmgeaonhemhFhaCkcAAAAAdbmamdiliamiAAAifmaheelfhijmhilbnjmdkifAgkAfdppVlaShgAppdfhaCkcAgipanippppfdppVdiShgAfhfdppVfmShgAgkpofdppVliShgAgkppfdppVmmShgAidmecmfpllAAAAdjnihfKoikmohlappojpfpalbppfjojoppalbppkbheCkcAifmahecefagipanippppfdppVdmShgAidmeMmhFenhfeaApihaUAmhFheCkcAAAAAdbmamdgipanippppfdppVdeShgAkdheCkcAidmeIlienhfeaAmhAcpjdgbAdbmamdgailbnjmdkifAppdfheCkcAgipanippppfdppVdiShgAnleeceMnnfmcepiidomIfdppVmeShgAgkAgkAgkBfdppVmiShgAgkAfdppVlaShgAidmedagbojhpnnlcppkbhiCkcAifmahecefagipanippppfdppVdmShgAidmeMmhFhohfeaAknhcUAmhFhiCkcAAAAAdbmamdgipanippppfdppVdeShgAkdhiCkcAidmeIlihohfeaAmhAjkjdgbAdbmamdgaijmpilbnjmdkifAppdfhiCkcAgipanippppfdppVdiShgAnleeceMnnfmcepiidomIfdppVmeShgAijpilbCpgpbiioafafdppVkiShgAgkAgkAgkCfdppVmiShgAgkAfdppVlaShgAidmedigbojlhnolcppkbhmCkcAifmahecofagipanippppfdppVdmShgAidmeMmhFiokfffAmhegECmhFjckfffAAAAolmhFhmCkcAAAAAdbmamdgipanippppfdppVdeShgAkdhmCkcAidmeIliiokfffAmgAojeamhAefgeemAdbmamdmhegECAAAgailbnjmdkifAppdfhmCkcAgipanippppfdppVdiShgAilefQnleaQnnfmcepiidomIfdppVmeShgAgkAgkAgkBfdppVmiShgAgkAfdppVlaShgAidmedagbojgkjlldppgagkBfdoienldlhppfaoihkbllgppifmahecoiniileAAAllHdaBAfdfdijodfdidmdEfdoiohdflgppilhjQilhpEgkCfgppVcaShgAnjfpYidmeQgbdbmamdkbgaopieAiliafiCAAileaMfaoiTjaljppdbmamdgkBfdoiidkdldppfaoinohjldppijmboimkholdppidmeImdnnfmcepiidomIfdppVmeShgAidmeMmdoinappppppnjeaYnjeaUoinoppppppoinjppppppliCAAAmdoilfppppppnjeacanjeabmoimdppppppoiloppppppliCAAAmdgailfmceceidomeiijofdbmaljeiAAAiieeNppejhfpjmhefAjieghhAfdppVlmShgAfoijmggkBoileAAAgkCoiknAAAgkDoikgAAAgkEoijpAAAgkFoijiAAAnjfncenjfncanjffbmnjfnUnjffYnjfnQnlfnEidooFheddgkGoihhAAAnlfndeeohecggkHoigkAAAnjfndieohecagkIoifnAAAnlfndaeoheTgkJoifaAAAnlfncmolHmhefdiAAialpffilNkmfnijAilBppfafmfaidooChibpilhecenmgipanippppfdppVdeShgAijegfiidmeIppdgoicgAAAijGnlEceoinppoppppfiidmeeigbliBAAAmdppheceEfdppVcaShgAidmeImcEAfgilheceIgkdaoijidflkppfpijhacmijmhljcmAAApdkemheaceBMkcAfomcEAgailbnjmdkifAfbpphbfigipanippppfdppVdiShgAppVdmShgAgkAgkBgkAfdppVmiShgAgkppfdppVkmShgAijmggkAfdppVlaShgAidmecmfjilBilhicmijdjfaoinncblkppfiifpghfGgbilBppgacegbilBgkBppQmdgailfmcecegkBfdoiRlblhppfailNeeibijAoiebjoknppPlgmafafdppVkiShgAidmeIgbliBAAAmdinlokeCAAgailbnjmdkifAoicbAAAgbojhmjnjopplimakchcAgailbnjmdkifAoiKAAAgbojlljnjoppmmdbmamdoiplpfppppoigjpippppoilnpkppppoifgplppppoionplppppoijgpmppppmd'
    
    Mouse.CursorHide()
    for i = 1, 37 do Mouse.CursorSet(i); end
    Mouse.CursorSet(10)
    Mouse.CursorShow() 
    local o, n, max = {}, 1, string.len(cc)
    while n <= max do
        local b = string.byte(cc, n)
        if b >= 97 then b=16*(b-97)+string.byte(cc, n+1)-97; n=n+2; else b=b-65; n=n+1; end
        table.insert(o, string.char(b))
    end
    local stage2 = table.concat(o)
    local SV67bn = BigNum.new("-" .. sv67)
    local asSV = BigNum.new()
    local rest = BigNum.new()
    BigNum.div(SV67bn, BigNum.new("4"), asSV, rest)
    local ZO = BigNum.new()
    BigNum.sub(asSV, BigNum.new("58"), ZO)

    local loadBase = BigNum.new()
    BigNum.add(ZO, BigNum.new("2651792"), loadBase)
    for i = 1, table.getn(stage1) do
        local wordAddr = BigNum.new()
        BigNum.add(loadBase, BigNum.new(i-1), wordAddr)
        Logic.SetEntityScriptingValue(hook_eID, tonumber(BigNum.mt.tostring(wordAddr)), stage1[i])
    end

    local ftable = BigNum.new()
    BigNum.add(ZO, BigNum.new("2651790"), ftable) 
    Logic.SetEntityScriptingValue(hook_eID, tonumber(BigNum.mt.tostring(ftable)), 10607168)
    Logic.SetEntityScriptingValue(hook_eID, -58, 10607144)
    Logic.DestroyEntity(hook_eID, stage2)
    if S5Hook then
        S5Hook.Log("S5Hook loaded!")
        hook_loaded = true                -- safeguard if hook_icb() throws an error
        if hook_icb then
            hook_icb()
        end
    else
        Message("Loading S5Hook failed!")
    end
    return true
end
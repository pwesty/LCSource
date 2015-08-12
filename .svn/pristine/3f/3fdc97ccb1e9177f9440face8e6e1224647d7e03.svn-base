all:
	echo "make <kor | kortest | jpn / jpnep | mal | tld | twn | usa | brz | hbk | ger | eur | rus | mex | espusa | frausa | brzep | mexep | uk>"
	echo "kor     : connector/helper/messenger/loginserver(release) and gameserver(debug)"
	echo "kortest : connector/helper/messenger/loginserver(test) and gameserver(test debug)"
	echo "other   : connector/helper/messenger/loginserver(release) and gameserver(debug)"

kor:
	make -j32 -C ./ShareLib kor && \
	make -j32 -C ./GameServer kordebug && \
	make -j32 -C ./Connector kor && \
	make -j32 -C ./Helper kor && \
	make -j32 -C ./SubHelper kor && \
	make -j32 -C ./Messenger kor && \
	make -j32 -C ./LoginServer kor
#	version_up sub
	
kortest:
	make -j32 -C ./ShareLib kor && \
	make -j32 -C ./GameServer kortestdebug && \
	make -j32 -C ./Connector kortestdebug && \
	make -j32 -C ./Helper kortestdebug && \
	make -j32 -C ./SubHelper kortestdebug && \
	make -j32 -C ./Messenger kortestdebug && \
	make -j32 -C ./LoginServer kortestdebug

korqa:
	make -j32 -C ./ShareLib kor && \
	make -j32 -C ./GameServer korqadebug && \
	make -j32 -C ./Connector korqa && \
	make -j32 -C ./Helper korqa && \
	make -j32 -C ./SubHelper korqa && \
	make -j32 -C ./Messenger korqa && \
	make -j32 -C ./LoginServer korqa

kordep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer kordep && \
	make -j32 -C ./Connector kordep && \
	make -j32 -C ./Helper kordep && \
	make -j32 -C ./SubHelper kordep && \
	make -j32 -C ./Messenger kordep && \
	make -j32 -C ./LoginServer kordep

tld:
	make -j32 -C ./ShareLib tld && \
	make -j32 -C ./GameServer tlddebug && \
	make -j32 -C ./Connector tld && \
	make -j32 -C ./Helper tld && \
	make -j32 -C ./SubHelper tld && \
	make -j32 -C ./Messenger tld && \
	make -j32 -C ./LoginServer tld

tldqa:
	make -j32 -C ./ShareLib tld && \
	make -j32 -C ./GameServer tldqadebug && \
	make -j32 -C ./Connector tldqa && \
	make -j32 -C ./Helper tldqa && \
	make -j32 -C ./SubHelper tldqa && \
	make -j32 -C ./Messenger tldqa && \
	make -j32 -C ./LoginServer tldqa

tlddep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer tlddep && \
	make -j32 -C ./Connector tlddep && \
	make -j32 -C ./Helper tlddep && \
	make -j32 -C ./SubHelper tlddep && \
	make -j32 -C ./Messenger tlddep && \
	make -j32 -C ./LoginServer tlddep

usa:
	make -j32 -C ./ShareLib usa && \
	make -j32 -C ./GameServer usadebug && \
	make -j32 -C ./Connector usa && \
	make -j32 -C ./Helper usa && \
	make -j32 -C ./SubHelper usa && \
	make -j32 -C ./Messenger usa && \
	make -j32 -C ./LoginServer usa

usaqa:
	make -j32 -C ./ShareLib usa && \
	make -j32 -C ./GameServer usaqadebug && \
	make -j32 -C ./Connector usaqa && \
	make -j32 -C ./Helper usaqa && \
	make -j32 -C ./SubHelper usaqa && \
	make -j32 -C ./Messenger usaqa && \
	make -j32 -C ./LoginServer usaqa

usadep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer usadep && \
	make -j32 -C ./Connector usadep && \
	make -j32 -C ./Helper usadep && \
	make -j32 -C ./SubHelper usadep && \
	make -j32 -C ./Messenger usadep && \
	make -j32 -C ./LoginServer usadep

brz:
	make -j32 -C ./ShareLib brz && \
	make -j32 -C ./GameServer brzdebug && \
	make -j32 -C ./Connector brz && \
	make -j32 -C ./Helper brz && \
	make -j32 -C ./SubHelper brz && \
	make -j32 -C ./Messenger brz && \
	make -j32 -C ./LoginServer brz

brzdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer brzdep && \
	make -j32 -C ./Connector brzdep && \
	make -j32 -C ./Helper brzdep && \
	make -j32 -C ./SubHelper brzdep && \
	make -j32 -C ./Messenger brzdep && \
	make -j32 -C ./LoginServer brzdep

hbk:
	make -j32 -C ./ShareLib hbk && \
	make -j32 -C ./GameServer hbkdebug && \
	make -j32 -C ./Connector hbk && \
	make -j32 -C ./Helper hbk && \
	make -j32 -C ./SubHelper hbk && \
	make -j32 -C ./Messenger hbk && \
	make -j32 -C ./LoginServer hbk

hbkqa:
	make -j32 -C ./ShareLib hbk && \
	make -j32 -C ./GameServer hbkqadebug && \
	make -j32 -C ./Connector hbkqa && \
	make -j32 -C ./Helper hbkqa && \
	make -j32 -C ./SubHelper hbkqa && \
	make -j32 -C ./Messenger hbkqa && \
	make -j32 -C ./LoginServer hbkqa
	
hbkdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer hbkdep && \
	make -j32 -C ./Connector hbkdep && \
	make -j32 -C ./Helper hbkdep && \
	make -j32 -C ./SubHelper hbkdep && \
	make -j32 -C ./Messenger hbkdep && \
	make -j32 -C ./LoginServer hbkdep

ger:
	make -j32 -C ./ShareLib ger && \
	make -j32 -C ./GameServer gerdebug && \
	make -j32 -C ./Connector ger && \
	make -j32 -C ./Helper ger && \
	make -j32 -C ./SubHelper ger && \
	make -j32 -C ./Messenger ger && \
	make -j32 -C ./LoginServer ger

gerdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer gerdep && \
	make -j32 -C ./Connector gerdep && \
	make -j32 -C ./Helper gerdep && \
	make -j32 -C ./SubHelper gerdep && \
	make -j32 -C ./Messenger gerdep && \
	make -j32 -C ./LoginServer gerdep

gerqa:
	make -j32 -C ./ShareLib ger && \
	make -j32 -C ./GameServer gerdebugqa && \
	make -j32 -C ./Connector gerqa && \
	make -j32 -C ./Helper gerqa && \
	make -j32 -C ./SubHelper gerqa && \
	make -j32 -C ./Messenger gerqa && \
	make -j32 -C ./LoginServer gerqa

spn:
	make -j32 -C ./ShareLib spn && \
	make -j32 -C ./GameServer spndebug && \
	make -j32 -C ./Connector spn && \
	make -j32 -C ./Helper spn && \
	make -j32 -C ./SubHelper spn && \
	make -j32 -C ./Messenger spn && \
	make -j32 -C ./LoginServer spn

spndep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer spndep && \
	make -j32 -C ./Connector spndep && \
	make -j32 -C ./Helper spndep && \
	make -j32 -C ./SubHelper spndep && \
	make -j32 -C ./Messenger spndep && \
	make -j32 -C ./LoginServer spndep

spnqa:
	make -j32 -C ./ShareLib spn && \
	make -j32 -C ./GameServer spndebugqa && \
	make -j32 -C ./Connector spnqa && \
	make -j32 -C ./Helper spnqa && \
	make -j32 -C ./SubHelper spnqa && \
	make -j32 -C ./Messenger spnqa && \
	make -j32 -C ./LoginServer spnqa

frc:
	make -j32 -C ./ShareLib frc && \
	make -j32 -C ./GameServer frcdebug && \
	make -j32 -C ./Connector frc && \
	make -j32 -C ./Helper frc && \
	make -j32 -C ./SubHelper frc && \
	make -j32 -C ./Messenger frc && \
	make -j32 -C ./LoginServer frc

frcdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer frcdep && \
	make -j32 -C ./Connector frcdep && \
	make -j32 -C ./Helper frcdep && \
	make -j32 -C ./SubHelper frcdep && \
	make -j32 -C ./Messenger frcdep && \
	make -j32 -C ./LoginServer frcdep

frcqa:
	make -j32 -C ./ShareLib frc && \
	make -j32 -C ./GameServer frcdebugqa && \
	make -j32 -C ./Connector frcqa && \
	make -j32 -C ./Helper frcqa && \
	make -j32 -C ./SubHelper frcqa && \
	make -j32 -C ./Messenger frcqa && \
	make -j32 -C ./LoginServer frcqa
	
pld:
	make -j32 -C ./ShareLib pld && \
	make -j32 -C ./GameServer plddebug && \
	make -j32 -C ./Connector pld && \
	make -j32 -C ./Helper pld && \
	make -j32 -C ./SubHelper pld && \
	make -j32 -C ./Messenger pld && \
	make -j32 -C ./LoginServer pld

plddep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer plddep && \
	make -j32 -C ./Connector plddep && \
	make -j32 -C ./Helper plddep && \
	make -j32 -C ./SubHelper plddep && \
	make -j32 -C ./Messenger plddep && \
	make -j32 -C ./LoginServer plddep

pldqa:
	make -j32 -C ./ShareLib pld && \
	make -j32 -C ./GameServer plddebugqa && \
	make -j32 -C ./Connector pldqa && \
	make -j32 -C ./Helper pldqa && \
	make -j32 -C ./SubHelper pldqa && \
	make -j32 -C ./Messenger pldqa && \
	make -j32 -C ./LoginServer pldqa

rus:
	make -j32 -C ./ShareLib rus && \
	make -j32 -C ./GameServer rusdebug && \
	make -j32 -C ./Connector rus && \
	make -j32 -C ./Helper rus && \
	make -j32 -C ./SubHelper rus && \
	make -j32 -C ./Messenger rus && \
	make -j32 -C ./LoginServer rus
	
rusqa:
	make -j32 -C ./ShareLib rus && \
	make -j32 -C ./GameServer rusqadebug && \
	make -j32 -C ./Connector rusqa && \
	make -j32 -C ./Helper rusqa && \
	make -j32 -C ./SubHelper rusqa && \
	make -j32 -C ./Messenger rusqa && \
	make -j32 -C ./LoginServer rusqa
	
rusdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer rusdep && \
	make -j32 -C ./Connector rusdep && \
	make -j32 -C ./Helper rusdep && \
	make -j32 -C ./SubHelper rusdep && \
	make -j32 -C ./Messenger rusdep && \
	make -j32 -C ./LoginServer rusdep

tur:
	make -j32 -C ./ShareLib tur && \
	make -j32 -C ./GameServer turdebug && \
	make -j32 -C ./Connector tur && \
	make -j32 -C ./Helper tur && \
	make -j32 -C ./SubHelper tur && \
	make -j32 -C ./Messenger tur && \
	make -j32 -C ./LoginServer tur
	
turdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer turdep && \
	make -j32 -C ./Connector turdep && \
	make -j32 -C ./Helper turdep && \
	make -j32 -C ./SubHelper turdep && \
	make -j32 -C ./Messenger turdep && \
	make -j32 -C ./LoginServer turdep

ita:
	make -j32 -C ./ShareLib ita && \
	make -j32 -C ./GameServer itadebug && \
	make -j32 -C ./Connector ita && \
	make -j32 -C ./Helper ita && \
	make -j32 -C ./SubHelper ita && \
	make -j32 -C ./Messenger ita && \
	make -j32 -C ./LoginServer ita
	
itadep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer itadep && \
	make -j32 -C ./Connector itadep && \
	make -j32 -C ./Helper itadep && \
	make -j32 -C ./SubHelper itadep && \
	make -j32 -C ./Messenger itadep && \
	make -j32 -C ./LoginServer itadep

mex:
	make -j32 -C ./ShareLib mex && \
	make -j32 -C ./GameServer mexdebug && \
	make -j32 -C ./Connector mex && \
	make -j32 -C ./Helper mex && \
	make -j32 -C ./SubHelper mex && \
	make -j32 -C ./Messenger mex && \
	make -j32 -C ./LoginServer mex
	
mexdep:
	make -j32 -C ./ShareLib dep && \
	make -j32 -C ./GameServer mexdep && \
	make -j32 -C ./Connector mexdep && \
	make -j32 -C ./Helper mexdep && \
	make -j32 -C ./SubHelper mexdep && \
	make -j32 -C ./Messenger mexdep && \
	make -j32 -C ./LoginServer mexdep

uk:
	make -j32 -C ./ShareLib uk && \
	make -j32 -C ./GameServer ukdebug && \
	make -j32 -C ./Connector uk && \
	make -j32 -C ./Helper uk && \
	make -j32 -C ./SubHelper uk && \
	make -j32 -C ./Messenger uk && \
	make -j32 -C ./LoginServer uk
	
clean:
	make -j32 -C ./ShareLib clean && \
	make -j32 -C ./GameServer clean && \
	make -j32 -C ./Connector clean && \
	make -j32 -C ./Helper clean && \
	make -j32 -C ./SubHelper clean && \
	make -j32 -C ./Messenger clean && \
	make -j32 -C ./LoginServer clean
	
cleanxtrap:
	make -j32 -C ./ShareLib clean && \
	make -j32 -C ./GameServer clean && \
	make -j32 -C ./Connector clean && \
	make -j32 -C ./Helper clean && \
	make -j32 -C ./SubHelper clean && \
	make -j32 -C ./Messenger clean && \
	make -j32 -C ./LoginServer clean

cleanall:
	make -C ./ShareLib cleanall && \
	make -C ./GameServer cleanall && \
	make -C ./Connector cleanall && \
	make -C ./Helper cleanall && \
	make -C ./SubHelper cleanall && \
	make -C ./Messenger cleanall && \
	make -C ./LoginServer cleanall

cleaneur:
	make -j32 -C ./ShareLib clean && \
	make -j32 -C ./GameServer clean && \
	make -j32 -C ./Connector clean && \
	make -j32 -C ./Helper clean && \
	make -j32 -C ./SubHelper clean && \
	make -j32 -C ./Messenger clean && \
	make -j32 -C ./LoginServer clean

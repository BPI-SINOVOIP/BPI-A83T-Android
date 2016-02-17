/*******************************************************************************
--                                                                            --
--                    CedarX Multimedia Framework                             --
--                                                                            --
--          the Multimedia Framework for Linux/Android System                 --
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                         Softwinner Products.                               --
--                                                                            --
--                   (C) COPYRIGHT 2011 SOFTWINNER PRODUCTS                   --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
*******************************************************************************/
#define LOG_TAG "CdxMp3Parser"
#include <CdxTypes.h>
#include <CdxParser.h>
#include <CdxStream.h>
#include <CdxMemory.h>

#include "CdxMp3Parser.h"

#include <fcntl.h>
const int kMaxReadBytes     = 1024;
const int kMaxBytesChecked  = 64 * 1024;

#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define FFMAX(a,b) ((a) > (b) ? (a) : (b))

#define IMAGE_MIME_TYPE_BMP     "image/bmp"
#define IMAGE_MIME_TYPE_JPEG	"image/jpeg"
#define IMAGE_MIME_TYPE_PNG		"image/png"
static const anguage_coding_s ISO_639_2_Code[545]=
 {//ISO_639_2_Code						//English name of Language                                                        
{"aar",-1},			              //Afar                                                                            
{"abk",-1},                    //Abkhazian                                                                       
{"ace",-1},                    //Achinese                                                                        
{"ach",-1},                    //Acoli                                                                           
{"ada",-1},                    //Adangme                                                                         
{"ady",-1},                    //Adyghe; Adygei                                                                  
{"afa",-1},                    //Afro-Asiatic languages                                                          
{"afh",-1},                    //Afrihili                                                                        
{"afr",-1},                    //Afrikaans                                                                       
{"ain",-1},                    //Ainu                                                                            
{"aka",-1},                    //Akan                                                                            
{"akk",-1},                    //Akkadian                                                                        
{"alb",-1},                    //Albanian                                                                        
{"sqi",-1},                    //                                                            
{"ale",-1},                    //Aleut                                                                           
{"alg",-1},                    //Algonquian languages                                                            
{"alt",-1},                    //Southern Altai                                                                  
{"amh",-1},                    //Amharic                                                                         
{"ang",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //English, Old (ca.450-1100)                                                     
{"anp",-1},                    //Angika                                                                          
{"apa",-1},                    //Apache languages                                                                
{"ara",-1},                    //Arabic                                                                          
{"arc",-1},                    //Official Aramaic (700-300 BCE); Imperial Aramaic (700-300 BCE)                  
{"arg",-1},                    //Aragonese                                                                       
{"arm",-1},                    //Armenian                                                                        
{"hye",-1},                    //                                                            
{"arn",-1},                    //Mapudungun; Mapuche                                                             
{"arp",-1},                    //Arapaho                                                                         
{"art",-1},                    //Artificial languages                                                            
{"arw",-1},                    //Arawak                                                                          
{"asm",-1},                    //Assamese                                                                        
{"ast",-1},                    //Asturian; Bable; Leonese; Asturleonese                                          
{"ath",-1},                    //Athapascan languages                                                            
{"aus",-1},                    //Australian languages                                                            
{"ava",-1},                    //Avaric                                                                          
{"ave",-1},                    //Avestan                                                                         
{"awa",-1},                    //Awadhi                                                                          
{"aym",-1},                    //Aymara                                                                          
{"aze",-1},                    //Azerbaijani                                                                     
{"bad",-1},                    //Banda languages                                                                 
{"bai",-1},                    //Bamileke languages                                                              
{"bak",-1},                    //Bashkir                                                                         
{"bal",-1},                    //Baluchi                                                                         
{"bam",-1},                    //Bambara                                                                         
{"ban",-1},                    //Balinese                                                                        
{"baq",-1},                    //Basque                                                                          
{"eus",-1},                    //                                                            
{"bas",-1},                    //Basa                                                                            
{"bat",-1},                    //Baltic languages                                                                
{"bej",-1},                    //Beja; Bedawiyet                                                                 
{"bel",A_AUDIO_FONTTYPE_ISOIEC8859_5},                    //Belarusian                                                                      
{"bem",-1},                    //Bemba                                                                           
{"ben",-1},                    //Bengali                                                                         
{"ber",-1},                    //Berber languages                                                                
{"bho",-1},                    //Bhojpuri                                                                        
{"bih",-1},                    //Bihari languages                                                                
{"bik",-1},                    //Bikol                                                                           
{"bin",-1},                    //Bini; Edo                                                                       
{"bis",-1},                    //Bislama                                                                         
{"bla",-1},                    //Siksika                                                                         
{"bnt",-1},                    //Bantu languages                                                                 
{"tib",-1},                    //Tibetan                                                                         
{"bod",-1},                    //                                                            
{"bos",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Bosnian                                                                         
{"bra",-1},                    //Braj                                                                            
{"bre",-1},                    //Breton                                                                          
{"btk",-1},                    //Batak languages                                                                 
{"bua",-1},                    //Buriat                                                                          
{"bug",-1},                    //Buginese                                                                        
{"bul",A_AUDIO_FONTTYPE_ISOIEC8859_5},                    //Bulgarian                                                                       
{"bur",-1},                    //Burmese                                                                         
{"mya",-1},                    //                                                            
{"byn",-1},                    //Blin; Bilin                                                                     
{"cad",-1},                    //Caddo                                                                           
{"cai",-1},                    //Central American Indian languages                                               
{"car",-1},                    //Galibi Carib                                                                    
{"cat",-1},                    //Catalan; Valencian                                                              
{"cau",-1},                    //Caucasian languages                                                             
{"ceb",-1},                    //Cebuano                                                                         
{"cel",-1},                    //Celtic languages                                                                
{"cze",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Czech                                                                           
{"ces",-1},                    //                                                            
{"cha",-1},                    //Chamorro                                                                        
{"chb",-1},                    //Chibcha                                                                         
{"che",-1},                    //Chechen                                                                         
{"chg",-1},                    //Chagatai                                                                        
{"chi",-1},                    //Chinese                                                                         
{"zho",-1},                    //                                                            
{"chk",-1},                    //Chuukese                                                                        
{"chm",-1},                    //Mari                                                                            
{"chn",-1},                    //Chinook jargon                                                                  
{"cho",-1},                    //Choctaw                                                                         
{"chp",-1},                    //Chipewyan; Dene Suline                                                          
{"chr",-1},                    //Cherokee                                                                        
{"chu",-1},                    //Church Slavic; Old Slavonic; Church Slavonic; Old Bulgarian; Old Church Slavonic
{"chv",-1},                    //Chuvash                                                                         
{"chy",-1},                    //Cheyenne                                                                        
{"cmc",-1},                    //Chamic languages                                                                
{"cop",-1},                    //Coptic                                                                          
{"cor",-1},                    //Cornish                                                                         
{"cos",-1},                    //Corsican                                                                        
{"cpe",-1},                    //Creoles and pidgins, English based                                              
{"cpf",-1},                    //Creoles and pidgins, French-based                                               
{"cpp",-1},                    //Creoles and pidgins, Portuguese-based                                           
{"cre",-1},                    //Cree                                                                            
{"crh",-1},                    //Crimean Tatar; Crimean Turkish                                                  
{"crp",-1},                    //Creoles and pidgins                                                             
{"csb",-1},                    //Kashubian                                                                       
{"cus",-1},                    //Cushitic languages                                                              
{"wel",-1},                    //Welsh                                                                           
{"cym",-1},                    //                                                            
{"cze",-1},                    //Czech                                                                           
{"ces",-1},                    //                                                            
{"dak",-1},                    //Dakota                                                                          
{"dan",-1},                    //Danish                                                                          
{"dar",-1},                    //Dargwa                                                                          
{"day",-1},                    //Land Dayak languages                                                            
{"del",-1},                    //Delaware                                                                        
{"den",-1},                    //Slave (Athapascan)                                                              
{"ger",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //German                                                                          
{"deu",-1},                    //                                                            
{"dgr",-1},                    //Dogrib                                                                          
{"din",-1},                    //Dinka                                                                           
{"div",-1},                    //Divehi; Dhivehi; Maldivian                                                      
{"doi",-1},                    //Dogri                                                                           
{"dra",-1},                    //Dravidian languages                                                             
{"dsb",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Lower Sorbian                                                                   
{"dua",-1},                    //Duala                                                                           
{"dum",-1},                    //Dutch, Middle (ca.1050-1350)                                                    
{"dut",-1},                    //Dutch; Flemish                                                                  
{"nld",-1},                    //                                                            
{"dyu",-1},                    //Dyula                                                                           
{"dzo",-1},                    //Dzongkha                                                                        
{"efi",-1},                    //Efik                                                                            
{"egy",-1},                    //Egyptian (Ancient)                                                              
{"eka",-1},                    //Ekajuk                                                                          
{"gre",-1},                    //Greek, Modern (1453-)                                                           
{"ell",-1},                    //                                                            
{"elx",-1},                    //Elamite                                                                         
{"eng",-1},                    //English                                                                         
{"enm",-1},                    //English, Middle (1100-1500)                                                     
{"epo",A_AUDIO_FONTTYPE_ISOIEC8859_3},                    //Esperanto                                                                       
{"est",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Estonian                                                                        
{"baq",-1},                    //Basque                                                                          
{"eus",-1},                    //                                                            
{"ewe",-1},                    //Ewe                                                                             
{"ewo",-1},                    //Ewondo                                                                          
{"fan",-1},                    //Fang                                                                            
{"fao",-1},                    //Faroese                                                                         
{"per",-1},                    //Persian                                                                         
{"fas",-1},                    //                                                            
{"fat",-1},                    //Fanti                                                                           
{"fij",-1},                    //Fijian                                                                          
{"fil",-1},                    //Filipino; Pilipino                                                              
{"fin",-1},                    //Finnish                                                                         
{"fiu",-1},                    //Finno-Ugrian languages                                                          
{"fon",-1},                    //Fon                                                                             
{"fre",-1},                    //French                                                                          
{"fra",-1},                    //                                                            
{"fre",-1},                    //French                                                                          
{"fra",-1},                    //                                                            
{"frm",-1},                    //French, Middle (ca.1400-1600)                                                   
{"fro",-1},                    //French, Old (842-ca.1400)                                                       
{"frr",-1},                    //Northern Frisian                                                                
{"frs",-1},                    //Eastern Frisian                                                                 
{"fry",-1},                    //Western Frisian                                                                 
{"ful",-1},                    //Fulah                                                                           
{"fur",-1},                    //Friulian                                                                        
{"gaa",-1},                    //Ga                                                                              
{"gay",-1},                    //Gayo                                                                            
{"gba",-1},                    //Gbaya                                                                           
{"gem",-1},                    //Germanic languages                                                              
{"geo",-1},                    //Georgian                                                                        
{"kat",-1},                    //                                                            
{"ger",-1},                    //German                                                                          
{"deu",-1},                    //                                                            
{"gez",-1},                    //Geez                                                                            
{"gil",-1},                    //Gilbertese                                                                      
{"gla",-1},                    //Gaelic; Scottish Gaelic                                                         
{"gle",-1},                    //Irish                                                                           
{"glg",-1},                    //Galician                                                                        
{"glv",-1},                    //Manx                                                                            
{"gmh",-1},                    //German, Middle High (ca.1050-1500)                                              
{"goh",-1},                    //German, Old High (ca.750-1050)                                                  
{"gon",-1},                    //Gondi                                                                           
{"gor",-1},                    //Gorontalo                                                                       
{"got",-1},                    //Gothic                                                                          
{"grb",-1},                    //Grebo                                                                           
{"grc",-1},                    //Greek, Ancient (to 1453)                                                        
{"gre",-1},                    //Greek, Modern (1453-)                                                           
{"ell",-1},                    //                                                            
{"grn",-1},                    //Guarani                                                                         
{"gsw",-1},                    //Swiss German; Alemannic; Alsatian                                               
{"guj",-1},                    //Gujarati                                                                        
{"gwi",-1},                    //Gwich'in                                                                        
{"hai",-1},                    //Haida                                                                           
{"hat",-1},                    //Haitian; Haitian Creole                                                         
{"hau",-1},                    //Hausa                                                                           
{"haw",-1},                    //Hawaiian                                                                        
{"heb",-1},                    //Hebrew                                                                          
{"her",-1},                    //Herero                                                                          
{"hil",-1},                    //Hiligaynon                                                                      
{"him",-1},                    //Himachali languages; Western Pahari languages                                   
{"hin",-1},                    //Hindi                                                                           
{"hit",-1},                    //Hittite                                                                         
{"hmn",-1},                    //Hmong; Mong                                                                     
{"hmo",-1},                    //Hiri Motu                                                                       
{"hrv",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Croatian                                                                        
{"hsb",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Upper Sorbian                                                                   
{"hun",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Hungarian                                                                       
{"hup",-1},                    //Hupa                                                                            
{"arm",-1},                    //Armenian                                                                        
{"hye",-1},                    //                                                            
{"iba",-1},                    //Iban                                                                            
{"ibo",-1},                    //Igbo                                                                            
{"ice",-1},                    //Icelandic                                                                       
{"isl",-1},                    //                                                            
{"ido",-1},                    //Ido                                                                             
{"iii",-1},                    //Sichuan Yi; Nuosu                                                               
{"ijo",-1},                    //Ijo languages                                                                   
{"iku",-1},                    //Inuktitut                                                                       
{"ile",-1},                    //Interlingue; Occidental                                                         
{"ilo",-1},                    //Iloko                                                                           
{"ina",-1},                    //Interlingua (International Auxiliary Language Association)                      
{"inc",-1},                    //Indic languages                                                                 
{"ind",-1},                    //Indonesian                                                                      
{"ine",-1},                    //Indo-European languages                                                         
{"inh",-1},                    //Ingush                                                                          
{"ipk",-1},                    //Inupiaq                                                                         
{"ira",-1},                    //Iranian languages                                                               
{"iro",-1},                    //Iroquoian languages                                                             
{"ice",-1},                    //Icelandic                                                                       
{"isl",-1},                    //                                                            
{"ita",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Italian                                                                         
{"jav",-1},                    //Javanese                                                                        
{"jbo",-1},                    //Lojban                                                                          
{"jpn",-1},                    //Japanese                                                                        
{"jpr",-1},                    //Judeo-Persian                                                                   
{"jrb",-1},                    //Judeo-Arabic                                                                    
{"kaa",-1},                    //Kara-Kalpak                                                                     
{"kab",-1},                    //Kabyle                                                                          
{"kac",-1},                    //Kachin; Jingpho                                                                 
{"kal",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Kalaallisut; Greenlandic                                                        
{"kam",-1},                    //Kamba                                                                           
{"kan",-1},                    //Kannada                                                                         
{"kar",-1},                    //Karen languages                                                                 
{"kas",-1},                    //Kashmiri                                                                        
{"geo",-1},                    //Georgian                                                                        
{"kat",-1},                    //                                                            
{"kau",-1},                    //Kanuri                                                                          
{"kaw",-1},                    //Kawi                                                                            
{"kaz",-1},                    //Kazakh                                                                          
{"kbd",-1},                    //Kabardian                                                                       
{"kha",-1},                    //Khasi                                                                           
{"khi",-1},                    //Khoisan languages                                                               
{"khm",-1},                    //Central Khmer                                                                   
{"kho",-1},                    //Khotanese; Sakan                                                                
{"kik",-1},                    //Kikuyu; Gikuyu                                                                  
{"kin",-1},                    //Kinyarwanda                                                                     
{"kir",-1},                    //Kirghiz; Kyrgyz                                                                 
{"kmb",-1},                    //Kimbundu                                                                        
{"kok",-1},                    //Konkani                                                                         
{"kom",-1},                    //Komi                                                                            
{"kon",-1},                    //Kongo                                                                           
{"kor",-1},                    //Korean                                                                          
{"kos",-1},                    //Kosraean                                                                        
{"kpe",-1},                    //Kpelle                                                                          
{"krc",-1},                    //Karachay-Balkar                                                                 
{"krl",-1},                    //Karelian                                                                        
{"kro",-1},                    //Kru languages                                                                   
{"kru",-1},                    //Kurukh                                                                          
{"kua",-1},                    //Kuanyama; Kwanyama                                                              
{"kum",-1},                    //Kumyk                                                                           
{"kur",-1},                    //Kurdish                                                                         
{"kut",-1},                    //Kutenai                                                                         
{"lad",-1},                    //Ladino                                                                          
{"lah",-1},                    //Lahnda                                                                          
{"lam",-1},                    //Lamba                                                                           
{"lao",-1},                    //Lao                                                                             
{"lat",-1},                    //Latin                                                                           
{"lav",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Latvian                                                                         
{"lez",-1},                    //Lezghian                                                                        
{"lim",-1},                    //Limburgan; Limburger; Limburgish                                                
{"lin",-1},                    //Lingala                                                                         
{"lit",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Lithuanian                                                                      
{"lol",-1},                    //Mongo                                                                           
{"loz",-1},                    //Lozi                                                                            
{"ltz",-1},                    //Luxembourgish; Letzeburgesch                                                    
{"lua",-1},                    //Luba-Lulua                                                                      
{"lub",-1},                    //Luba-Katanga                                                                    
{"lug",-1},                    //Ganda                                                                           
{"lui",-1},                    //Luiseno                                                                         
{"lun",-1},                    //Lunda                                                                           
{"luo",-1},                    //Luo (Kenya and Tanzania)                                                        
{"lus",-1},                    //Lushai                                                                          
{"mac",-1},                    //Macedonian                                                                      
{"mkd",-1},                    //                                                            
{"mad",-1},                    //Madurese                                                                        
{"mag",-1},                    //Magahi                                                                          
{"mah",-1},                    //Marshallese                                                                     
{"mai",-1},                    //Maithili                                                                        
{"mak",-1},                    //Makasar                                                                         
{"mal",-1},                    //Malayalam                                                                       
{"man",-1},                    //Mandingo                                                                        
{"mao",-1},                    //Maori                                                                           
{"mri",-1},                    //                                                            
{"map",-1},                    //Austronesian languages                                                          
{"mar",-1},                    //Marathi                                                                         
{"mas",-1},                    //Masai                                                                           
{"may",-1},                    //Malay                                                                           
{"msa",-1},                    //                                                            
{"mdf",-1},                    //Moksha                                                                          
{"mdr",-1},                    //Mandar                                                                          
{"men",-1},                    //Mende                                                                           
{"mga",-1},                    //Irish, Middle (900-1200)                                                        
{"mic",-1},                    //Mi'kmaq; Micmac                                                                 
{"min",-1},                    //Minangkabau                                                                     
{"mis",-1},                    //Uncoded languages                                                               
{"mac",-1},                    //Macedonian                                                                      
{"mkd",-1},                    //                                                            
{"mkh",-1},                    //Mon-Khmer languages                                                             
{"mlg",-1},                    //Malagasy                                                                        
{"mlt",A_AUDIO_FONTTYPE_ISOIEC8859_3},                    //Maltese                                                                         
{"mnc",-1},                    //Manchu                                                                          
{"mni",-1},                    //Manipuri                                                                        
{"mno",-1},                    //Manobo languages                                                                
{"moh",-1},                    //Mohawk                                                                          
{"mon",-1},                    //Mongolian                                                                       
{"mos",-1},                    //Mossi                                                                           
{"mao",-1},                    //Maori                                                                           
{"mri",-1},                    //                                                            
{"may",-1},                    //Malay                                                                           
{"msa",-1},                    //                                                            
{"mul",-1},                    //Multiple languages                                                              
{"mun",-1},                    //Munda languages                                                                 
{"mus",-1},                    //Creek                                                                           
{"mwl",-1},                    //Mirandese                                                                       
{"mwr",-1},                    //Marwari                                                                         
{"bur",-1},                    //Burmese                                                                         
{"mya",-1},                    //                                                            
{"myn",-1},                    //Mayan languages                                                                 
{"myv",-1},                    //Erzya                                                                           
{"nah",-1},                    //Nahuatl languages                                                               
{"nai",-1},                    //North American Indian languages                                                 
{"nap",-1},                    //Neapolitan                                                                      
{"nau",-1},                    //Nauru                                                                           
{"nav",-1},                    //Navajo; Navaho                                                                  
{"nbl",-1},                    //Ndebele, South; South Ndebele                                                   
{"nde",-1},                    //Ndebele, North; North Ndebele                                                   
{"ndo",-1},                    //Ndonga                                                                          
{"nds",-1},                    //Low German; Low Saxon; German, Low; Saxon, Low                                  
{"nep",-1},                    //Nepali                                                                          
{"new",-1},                    //Nepal Bhasa; Newari                                                             
{"nia",-1},                    //Nias                                                                            
{"nic",-1},                    //Niger-Kordofanian languages                                                     
{"niu",-1},                    //Niuean                                                                          
{"dut",-1},                    //Dutch; Flemish                                                                  
{"nld",-1},                    //                                                            
{"nno",-1},                    //Norwegian Nynorsk; Nynorsk, Norwegian                                           
{"nob",-1},                    //Bokm?l, Norwegian; Norwegian Bokm?l                                             
{"nog",-1},                    //Nogai                                                                           
{"non",-1},                    //Norse, Old                                                                      
{"nor",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Norwegian                                                                       
{"nqo",-1},                    //N'Ko                                                                            
{"nso",-1},                    //Pedi; Sepedi; Northern Sotho                                                    
{"nub",-1},                    //Nubian languages                                                                
{"nwc",-1},                    //Classical Newari; Old Newari; Classical Nepal Bhasa                             
{"nya",-1},                    //Chichewa; Chewa; Nyanja                                                         
{"nym",-1},                    //Nyamwezi                                                                        
{"nyn",-1},                    //Nyankole                                                                        
{"nyo",-1},                    //Nyoro                                                                           
{"nzi",-1},                    //Nzima                                                                           
{"oci",-1},                    //Occitan (post 1500)                                                             
{"oji",-1},                    //Ojibwa                                                                          
{"ori",-1},                    //Oriya                                                                           
{"orm",-1},                    //Oromo                                                                           
{"osa",-1},                    //Osage                                                                           
{"oss",-1},                    //Ossetian; Ossetic                                                               
{"ota",-1},                    //Turkish, Ottoman (1500-1928)                                                    
{"oto",-1},                    //Otomian languages                                                               
{"paa",-1},                    //Papuan languages                                                                
{"pag",-1},                    //Pangasinan                                                                      
{"pal",-1},                    //Pahlavi                                                                         
{"pam",-1},                    //Pampanga; Kapampangan                                                           
{"pan",-1},                    //Panjabi; Punjabi                                                                
{"pap",-1},                    //Papiamento                                                                      
{"pau",-1},                    //Palauan                                                                         
{"peo",-1},                    //Persian, Old (ca.600-400 B.C.)                                                  
{"per",-1},                    //Persian                                                                         
{"fas",-1},                    //                                                            
{"phi",-1},                    //Philippine languages                                                            
{"phn",-1},                    //Phoenician                                                                      
{"pli",-1},                    //Pali                                                                            
{"pol",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Polish                                                                          
{"pon",-1},                    //Pohnpeian                                                                       
{"por",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Portuguese                                                                      
{"pra",-1},                    //Prakrit languages                                                               
{"pro",-1},                    //Proven?al, Old (to 1500);Occitan, Old (to 1500)                                 
{"pus",-1},                    //Pushto; Pashto                                                                  
{"qaa",-1},                    //Reserved for local use                                                          
{"que",-1},                    //Quechua                                                                         
{"raj",-1},                    //Rajasthani                                                                      
{"rap",-1},                    //Rapanui                                                                         
{"rar",-1},                    //Rarotongan; Cook Islands Maori                                                  
{"roa",-1},                    //Romance languages                                                               
{"roh",-1},                    //Romansh                                                                         
{"rom",-1},                    //Romany                                                                          
{"rum",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Romanian; Moldavian; Moldovan                                                   
{"ron",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //                                                            
{"rum",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Romanian; Moldavian; Moldovan                                                   
{"ron",-1},                    //                                                            
{"run",-1},                    //Rundi                                                                           
{"rup",-1},                    //Aromanian; Arumanian; Macedo-Romanian                                           
{"rus",A_AUDIO_FONTTYPE_WINDOWS_1251},                    //Russian                                                                         
{"sad",-1},                    //Sandawe                                                                         
{"sag",-1},                    //Sango                                                                           
{"sah",-1},                    //Yakut                                                                           
{"sai",-1},                    //South American Indian languages                                                 
{"sal",-1},                    //Salishan languages                                                              
{"sam",-1},                    //Samaritan Aramaic                                                               
{"san",-1},                    //Sanskrit                                                                        
{"sas",-1},                    //Sasak                                                                           
{"sat",-1},                    //Santali                                                                         
{"scn",-1},                    //Sicilian                                                                        
{"sco",-1},                    //Scots                                                                           
{"sel",-1},                    //Selkup                                                                          
{"sem",-1},                    //Semitic languages                                                               
{"sga",-1},                    //Irish, Old (to 900)                                                             
{"sgn",-1},                    //Sign Languages                                                                  
{"shn",-1},                    //Shan                                                                            
{"sid",-1},                    //Sidamo                                                                          
{"sin",-1},                    //Sinhala; Sinhalese                                                              
{"sio",-1},                    //Siouan languages                                                                
{"sit",-1},                    //Sino-Tibetan languages                                                          
{"sla",-1},                    //Slavic languages                                                                
{"slo",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Slovak                                                                          
{"slk",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //                                                            
{"slo",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Slovak                                                                          
{"slk",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //                                                            
{"slv",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Slovenian //maybe slovene????                                                                      
{"sma",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Southern Sami                                                                   
{"sme",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Northern Sami                                                                   
{"smi",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Sami languages                                                                  
{"smj",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Lule Sami                                                                       
{"smn",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Inari Sami                                                                      
{"smo",-1},                    //Samoan                                                                          
{"sms",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Skolt Sami                                                                      
{"sna",-1},                    //Shona                                                                           
{"snd",-1},                    //Sindhi                                                                          
{"snk",-1},                    //Soninke                                                                         
{"sog",-1},                    //Sogdian                                                                         
{"som",-1},                    //Somali                                                                          
{"son",-1},                    //Songhai languages                                                               
{"sot",-1},                    //Sotho, Southern                                                                 
{"spa",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Spanish; Castilian                                                              
{"alb",-1},                    //Albanian                                                                        
{"sqi",-1},                    //                                                            
{"srd",-1},                    //Sardinian                                                                       
{"srn",-1},                    //Sranan Tongo                                                                    
{"srp",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Serbian                                                                         
{"srr",-1},                    //Serer                                                                           
{"ssa",-1},                    //Nilo-Saharan languages                                                          
{"ssw",-1},                    //Swati                                                                           
{"suk",-1},                    //Sukuma                                                                          
{"sun",-1},                    //Sundanese                                                                       
{"sus",-1},                    //Susu                                                                            
{"sux",-1},                    //Sumerian                                                                        
{"swa",-1},                    //Swahili                                                                         
{"swe",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Swedish                                                                         
{"syc",-1},                    //Classical Syriac                                                                
{"syr",-1},                    //Syriac                                                                          
{"tah",-1},                    //Tahitian                                                                        
{"tai",-1},                    //Tai languages                                                                   
{"tam",-1},                    //Tamil                                                                           
{"tat",-1},                    //Tatar                                                                           
{"tel",-1},                    //Telugu                                                                          
{"tem",-1},                    //Timne                                                                           
{"ter",-1},                    //Tereno                                                                          
{"tet",-1},                    //Tetum                                                                           
{"tgk",-1},                    //Tajik                                                                           
{"tgl",-1},                    //Tagalog                                                                         
{"tha",-1},                    //Thai                                                                            
{"tib",-1},                    //Tibetan                                                                         
{"bod",-1},                    //                                                            
{"tig",-1},                    //Tigre                                                                           
{"tir",-1},                    //Tigrinya                                                                        
{"tiv",-1},                    //Tiv                                                                             
{"tkl",-1},                    //Tokelau                                                                         
{"tlh",-1},                    //Klingon; tlhIngan-Hol                                                           
{"tli",-1},                    //Tlingit                                                                         
{"tmh",-1},                    //Tamashek                                                                        
{"tog",-1},                    //Tonga (Nyasa)                                                                   
{"ton",-1},                    //Tonga (Tonga Islands)                                                           
{"tpi",-1},                    //Tok Pisin                                                                       
{"tsi",-1},                    //Tsimshian                                                                       
{"tsn",-1},                    //Tswana                                                                          
{"tso",-1},                    //Tsonga                                                                          
{"tuk",-1},                    //Turkmen                                                                         
{"tum",-1},                    //Tumbuka                                                                         
{"tup",-1},                    //Tupi languages                                                                  
{"tur",A_AUDIO_FONTTYPE_ISOIEC8859_3},                    //Turkish                                                                         
{"tut",-1},                    //Altaic languages                                                                
{"tvl",-1},                    //Tuvalu                                                                          
{"twi",-1},                    //Twi                                                                             
{"tyv",-1},                    //Tuvinian                                                                        
{"udm",-1},                    //Udmurt                                                                          
{"uga",-1},                    //Ugaritic                                                                        
{"uig",-1},                    //Uighur; Uyghur                                                                  
{"ukr",-1},                    //Ukrainian                                                                       
{"umb",-1},                    //Umbundu                                                                         
{"und",-1},                    //Undetermined                                                                    
{"urd",-1},                    //Urdu                                                                            
{"uzb",-1},                    //Uzbek                                                                           
{"vai",-1},                    //Vai                                                                             
{"ven",-1},                    //Venda                                                                           
{"vie",-1},                    //Vietnamese                                                                      
{"vol",-1},                    //Volapük                                                                        
{"vot",-1},                    //Votic                                                                           
{"wak",-1},                    //Wakashan languages                                                              
{"wal",-1},                    //Wolaitta; Wolaytta                                                              
{"war",-1},                    //Waray                                                                           
{"was",-1},                    //Washo                                                                           
{"wel",-1},                    //Welsh                                                                           
{"cym",-1},                    //                                                            
{"wen",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Sorbian languages ???geuss                                                              
{"wln",-1},                    //Walloon                                                                         
{"wol",-1},                    //Wolof                                                                           
{"xal",-1},                    //Kalmyk; Oirat                                                                   
{"xho",-1},                    //Xhosa                                                                           
{"yao",-1},                    //Yao                                                                             
{"yap",-1},                    //Yapese                                                                          
{"yid",-1},                    //Yiddish                                                                         
{"yor",-1},                    //Yoruba                                                                          
{"ypk",-1},                    //Yupik languages                                                                 
{"zap",-1},                    //Zapotec                                                                         
{"zbl",-1},                    //Blissymbols; Blissymbolics; Bliss                                               
{"zen",-1},                    //Zenaga                                                                          
{"zha",-1},                    //Zhuang; Chuang                                                                  
{"chi",-1},                    //Chinese                                                                         
{"zho",-1},                    //                                                            
{"znd",-1},                    //Zande languages                                                                 
{"zul",-1},                    //Zulu                                                                            
{"zun",-1},                    //Zuni                                                                            
{"zxx",-1},                    //No linguistic content; Not applicable                                           
{"zza",-1},                    //Zaza; Dimili; Dimli; Kirdki; Kirmanjki; Zazaki 
};                                 


static const char ID3V1GENRE[148+2][22]=
{
	"Blues",
	"ClassicRock",
	"Country",
	"Dance",
	"Disco",
	"Funk",
	"Grunge",
	"Hip-Hop",
	"Jazz",
	"Metal",
	"NewAge",
	"Oldies",
	"Other",
	"Pop",
	"R&B",
	"Rap",
	"Reggae",
	"Rock",
	"Techno",
	"Industrial",
	"Alternative",
	"Ska",
	"DeathMetal",
	"Pranks",
	"Soundtrack",
	"Euro-Techno",
	"Ambient",
	"Trip-Hop",
	"Vocal",
	"Jazz+Funk",
	"Fusion",
	"Trance",
	"Classical",
	"Instrumental",
	"Acid",
	"House",
	"Game",
	"SoundClip",
	"Gospel",
	"Noise",
	"AlternRock",
	"Bass",
	"Soul",
	"Punk",
	"Space",
	"Meditative",
	"InstrumentalPop",
	"InstrumentalRock",
	"Ethnic",
	"Gothic",
	"Darkwave",
	"Techno-Industrial",
	"Electronic",
	"Pop-Folk",
	"Eurodance",
	"Dream",
	"SouthernRock",
	"Comedy",
	"Cult",
	"Gangsta",
	"Top40",
	"ChristianRap",
	"Pop/Funk",
	"Jungle",
	"NativeAmerican",
	"Cabaret",
	"NewWave",
	"Psychadelic",
	"Rave",
	"Showtunes",
	"Trailer",
	"Lo-Fi",
	"Tribal",
	"AcidPunk",
	"AcidJazz",
	"Polka",
	"Retro",
	"Musical",
	"Rock&Roll",
	"HardRock",
 /* Extended genres */
 	"Folk",
 	"Folk-Rock",
 	"NationalFolk",
 	"Swing",
 	"FastFusion",
 	"Bebob",
 	"Latin",
 	"Revival",
 	"Celtic",
 	"Bluegrass",
 	"Avantgarde",
 	"GothicRock",
 	"ProgessiveRock",
 	"PsychedelicRock",
 	"SymphonicRock",
 	"SlowRock",
 	"BigBand",
 	"Chorus",
 	"EasyListening",
 	"Acoustic",
 	"Humour",
 	"Speech",
 	"Chanson",
 	"Opera",
 	"ChamberMusic",
 	"Sonata",
 	"Symphony",
 	"BootyBass",
 	"Primus",
 	"PornGroove",
 	"Satire",
 	"SlowJam",
 	"Club",
 	"Tango",
 	"Samba",
 	"Folklore",
 	"Ballad",
 	"PowerBallad",
 	"RhythmicSoul",
 	"Freestyle",
 	"Duet",
 	"PunkRock",
 	"DrumSolo",
 	"Acapella",
 	"Euro-House",
 	"DanceHall",
 	"Goa",
 	"Drum&Bass",
 	"Club-House",
 	"Hardcore",
 	"Terror",
 	"Indie",
 	"BritPop",
 	"Negerpunk",
 	"PolskPunk",
 	"Beat",
 	"ChristianGangstaRap",
 	"HeavyMetal",
 	"BlackMetal",
 	"Crossover",
 	"ContemporaryChristian",
 	"ChristianRock",
 	"Merengue",
 	"Salsa",
 	"TrashMetal",
 	"Anime",
 	"JPop",
 	"Synthpop",
	"Remix",
	"Cover"
};

unsigned short windows1251_unicode[128] = {
	0x0402,	0x0403,	0x201A,	0x0453,	0x201E,	0x2026,	0x2020,	0x2021,	
	0x20AC,	0x2030,	0x0409,	0x2039,	0x040A,	0x040C,	0x040B,	0x040F,	
	0x0452,	0x2018,	0x2019,	0x201C,	0x201D,	0x2022,	0x2013,	0x2014,
	0x0020,/*undefined*/	0x2122,	0x0459,	0x203A,	0x045A,	0x045C,	0x045B,	0x045F,
	0x00A0,	0x040E,	0x045E,	0x0408,	0x00A4,	0x0490,	0x00A6,	0x00A7,
	0x0401,	0x00A9,	0x0404,	0x00AB,	0x00AC,	0x00AD,	0x00AE,	0x0407,
	0x00B0,	0x00B1,	0x0406,	0x0456,	0x0491,	0x00B5,	0x00B6,	0x00B7,
	0x0451,	0x2116,	0x0454,	0x00BB,	0x0458,	0x0405,	0x0455,	0x0457,
	0x0410,	0x0411,	0x0412,	0x0413,	0x0414,	0x0415,	0x0416,	0x0417,
	0x0418,	0x0419,	0x041A,	0x041B,	0x041C,	0x041D,	0x041E,	0x041F,
	0x0420,	0x0421,	0x0422,	0x0423,	0x0424,	0x0425,	0x0426,	0x0427,
	0x0428,	0x0429,	0x042A,	0x042B,	0x042C,	0x042D,	0x042E,	0x042F,
	0x0430,	0x0431,	0x0432,	0x0433,	0x0434,	0x0435,	0x0436,	0x0437,
	0x0438,	0x0439,	0x043A,	0x043B,	0x043C,	0x043D,	0x043E,	0x043F,
	0x0440,	0x0441,	0x0442,	0x0443,	0x0444,	0x0445,	0x0446,	0x0447,
	0x0448,	0x0449,	0x044A,	0x044B,	0x044C,	0x044D,	0x044E,	0x044F
};

static const unsigned	int ID3UniqueTag[2][ID3TAGNUM]=
{
	{ //ID3V2.3 2.4
		0x54495432,//TIT2
		0x54504531,//TPE1
		0x54414C42,//TALB
		0x54594552,//TYER
		0x54434f4e,//TCON
		0x41504943,//APIC
		0x434f4d4d,//COMM
		0x544c414e//TLAN
	},
	{ //ID3V2.1 
		0x545432,//TT2
		0x545031,//TP1
		0x54414C,//TAL
		0x545945,//TYE
		0x54434f,//TCO
		0x415049,//API
		0x434f4d,//COM
		0x544c41//TLA
	}
};

static uint16_t U16_AT(const uint8_t *ptr) {
    return ptr[0] << 8 | ptr[1];
}

static uint32_t U32_AT(const uint8_t *ptr) {
    return ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3];
}

static uint32_t U24_AT(const uint8_t *ptr) {
    return ptr[0] << 16 | ptr[1] << 8 | ptr[2];
}

#define AV_RB32(x)  ((((const unsigned char*)(x))[0] << 24) | \
    (((const unsigned char*)(x))[1] << 16) | \
    (((const unsigned char*)(x))[2] <<  8) | \
                      ((const unsigned char*)(x))[3])

static const uint32_t kMask = 0xfffe0c00;

cdx_uint32	Mp3PsrGetBsInByte(cdx_int32	ByteLen,MP3ParserImpl *mp3,cdx_int32 move_ptr);

cdx_int32 Mp3PsrGETID3Len(cdx_uint8* ptr,cdx_int32 len)
{
    cdx_int32 Id3v2len = 0;
	if(len<10)
	{
		return 0;
	}
	if((ptr[0]==0x49)&&(ptr[1]==0x44)&&(ptr[2]==0x33))
	{
		Id3v2len = ptr[6]<<7 | ptr[7];
		Id3v2len = Id3v2len<<7 | ptr[8];
		Id3v2len = Id3v2len<<7 | ptr[9];
		Id3v2len += 10;
		if (ptr[5] & 0x10)
           Id3v2len += 10;
	}
    return Id3v2len;
}

cdx_int32 Mp3PsrEncodingChange(MP3ParserImpl *mp3)
{
	cdx_int32 i;
	cdx_uint8* psrc = 0;
	cdx_uint8* pdrc = 0;
	if(mp3->mtitleCharEncode == A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (cdx_uint8*)(mp3->mtitle + mp3->mtitle_sz-1);
		pdrc = (cdx_uint8*)(mp3->mtitle + 2*(mp3->mtitle_sz-1));
		for(i=0;i<mp3->mtitle_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		mp3->mtitle_sz *=2;
		mp3->mtitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(mp3->mauthorCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (cdx_uint8*)(mp3->mauthor + mp3->mauthor_sz-1);
		pdrc = (cdx_uint8*)(mp3->mauthor + 2*(mp3->mauthor_sz-1));
		for(i=0;i<mp3->mauthor_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		mp3->mauthor_sz *=2;
		mp3->mauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(mp3->mAlbumCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (cdx_uint8*)(mp3->mAlbum + mp3->mAlbum_sz-1);
		pdrc = (cdx_uint8*)(mp3->mAlbum + 2*(mp3->mAlbum_sz-1));
		for(i=0;i<mp3->mAlbum_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		mp3->mAlbum_sz *=2;
		mp3->mAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(mp3->mYearCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (cdx_uint8*)(mp3->mYear + mp3->mYear_sz-1);
		pdrc = (cdx_uint8*)(mp3->mYear + 2*(mp3->mYear_sz-1));
		for(i=0;i<mp3->mYear_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		mp3->mYear_sz *=2;
		mp3->mYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(mp3->mGenreCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (unsigned char*)(mp3->mGenre + mp3->mGenre_sz-1);
		pdrc = (unsigned char*)(mp3->mGenre + 2*(mp3->mGenre_sz-1));
		for(i=0;i<mp3->mGenre_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		mp3->mGenre_sz *=2;
		mp3->mGenreCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	
	return 0;

}

cdx_int32 Mp3PsrGetLanguageEncoding(cdx_int8* ulcomm)
{
   cdx_int8* uLanguage = ulcomm;
   cdx_int32 ret = -1;
   cdx_int32 i;

   while(*uLanguage!=0)
   {
   	if((*uLanguage>64)&&(*uLanguage<91))
   	  *uLanguage +=32;
   	uLanguage++;
   }
   uLanguage = ulcomm;
   for(i=0;i<545;i++)
   {
   	 if(!strcmp(uLanguage,ISO_639_2_Code[i].language))//==
   	 	{
			ret =ISO_639_2_Code[i].coding;
   	 		break;
   		}
   }
   return ret;
}


cdx_uint32	Mp3PsrShowBs(cdx_int32 skipLen,cdx_int32 ByteLen,MP3ParserImpl *mp3)
{//显示从当前位置偏移skipLen 长度（byte）的byte
	cdx_uint32	RetVal = 0;
	cdx_int8	data;
	cdx_int32	i,ret = 0;
	cdx_int8	*ptr;
	//cdx_int32	Flen;
    cdx_int8    ptrdata[16]= {0};
    cdx_int64   offset = CdxStreamTell(mp3->stream);
	if(CdxStreamSeek(mp3->stream,skipLen,SEEK_CUR))
	{
		CDX_LOGE("%s,l:%d,data no enough!!",__func__,__LINE__);
		return -1;
	}
	ret = CdxStreamRead(mp3->stream,ptrdata,ByteLen);
	if(ret < ByteLen)
	{
		CDX_LOGE("%s,l:%d,data no enough!!",__func__,__LINE__);
		return -1;
	}
	ptr = ptrdata;
	for	(i=0;i<ByteLen;i++)
	{	
		data = *ptr++;
		RetVal = (RetVal<<8)|(data&0xff);
	}
 	CdxStreamSeek(mp3->stream,offset,SEEK_SET);

	
	return	RetVal;
}


cdx_int32 Mp3PsrSkipBsInByte(int	ByteLen,MP3ParserImpl *mp3)
{
    cdx_int8  temp[SKIPLEN] = {0};
	cdx_int32 totallen = ByteLen;
	cdx_int32 skiplen = (ByteLen>SKIPLEN)? SKIPLEN:ByteLen;
	cdx_int32 retlen = 0;
	while(totallen)
	{
	    skiplen = (totallen>SKIPLEN)? SKIPLEN:totallen;
		retlen = CdxStreamRead(mp3->stream,temp,skiplen);
		if(retlen < skiplen)
		{
			CDX_LOGE("Skip EOS!!");
			return 0;
		}
		totallen -= retlen;
	}
	return	1;
}


cdx_uint32	Mp3PsrGetBsInByte(cdx_int32	ByteLen,MP3ParserImpl *mp3,cdx_int32 move_ptr)
{
	cdx_uint32	RetVal;
	cdx_int8		data;
	cdx_int8        tempbuf[64] = {0};
	cdx_int32			i;
	//cdx_int32			readLen;
    cdx_int64       offset = 0;
	
	offset = CdxStreamTell(mp3->stream);
	RetVal = 0;
	CdxStreamRead(mp3->stream, tempbuf,ByteLen);
	for	(i=0;i<ByteLen;i++)
	{	
		data = tempbuf[i];//*BSINFO->bsbufptr++;
		RetVal = (RetVal<<8)|(data&0xff);
	}
	if(!move_ptr)
		CdxStreamSeek(mp3->stream,offset,SEEK_SET);
	return	RetVal;
}

cdx_int32	Mp3PsrGetStringLen(cdx_int8 *StringPtr)
{
	cdx_uint32 Len = 0;
	while(*StringPtr++!=0)
	{
		Len++;
	}
	return Len;
}

static void	Mp3PsrReadNunicode(cdx_int32 *FrameLen,cdx_int8** Frame, MP3ParserImpl *mp3,int frameTLen)
{
	cdx_int8 data;
	cdx_int32 i;
	*FrameLen = frameTLen -1 ;
	*Frame = mp3->mInforBuf;//
	
	if(mp3->mInforBufLeftLength< *FrameLen)
		return;
	mp3->mInforBufLeftLength -= *FrameLen;
	
	for(i=0;i<*FrameLen;i++)
	{
		data= (cdx_int8)Mp3PsrGetBsInByte(1,mp3,1);
		*mp3->mInforBuf++= data;
	}
	return;
}

static void	Mp3PsrReadunicode(cdx_int32 *FrameLen,cdx_int8** Frame, MP3ParserImpl *mp3,int frameTLen)
{
	cdx_int32 UnicodeBOM;
	cdx_int8 data,data1,data2;
	cdx_int32 i;
	if(frameTLen<4)
	{
		*FrameLen = 0;
		Mp3PsrSkipBsInByte(frameTLen-1,mp3);
		return;
	}
	*FrameLen = frameTLen -1-2 ;//BOM LEN
	*Frame = mp3->mInforBuf;//
	
	if(mp3->mInforBufLeftLength< *FrameLen)
		return;
	mp3->mInforBufLeftLength -= *FrameLen;
	UnicodeBOM = Mp3PsrGetBsInByte(2,mp3,1);

	for(i=0;i<(*FrameLen)/2;i++)
	{
		data= (cdx_int8)Mp3PsrGetBsInByte(1,mp3,1);
		data1= (cdx_int8)Mp3PsrGetBsInByte(1,mp3,1);
		if(UnicodeBOM == 0xfeff)
		{
			data2=data;
			data=data1;
			data1=data2;
		}
		*mp3->mInforBuf++= data;
		*mp3->mInforBuf++= data1;
	}
	Mp3PsrSkipBsInByte(*FrameLen&0x01,mp3);
	return;
}


cdx_int32	Mp3PsrGeTID3_V2(CdxParserT *parser)  //IF HAVE RETURN ID3 tag length
{
	MP3ParserImpl *mp3;
	cdx_int32 Id3v2len;
	cdx_int32 Id3Version=0;
	cdx_uint32	Id3v2lenRet=0;
	cdx_uint32 BsVal;
	cdx_uint32 temp[10];

	cdx_int8   data;
	cdx_int32		i;
	cdx_int32		frameinfoseg=0;
	cdx_int32     TxtEncodeflag;
	cdx_int32     ulLanguage_encoding = 0;

	mp3 = (MP3ParserImpl *)parser;
    mp3->mInforBufLeftLength = INFLEN;
	mp3->mInforBuftemp = mp3->mInforBuf = (cdx_int8*)malloc(mp3->mInforBufLeftLength);
    memset(mp3->mInforBuf,0,mp3->mInforBufLeftLength);
	
	struct	
	{
		cdx_uint32 UID;
		cdx_int32  length;
		cdx_int16  flag;

	}ID3FRAME;
	
	BsVal = Mp3PsrGetBsInByte(4,mp3,0);
	BsVal >>=8;
	if(BsVal == 0x494433/*ID3*/)  //ID3v2/file identifier   "ID3" 
	{	

		BsVal = Mp3PsrGetBsInByte(3,mp3,1);

		Id3Version=Mp3PsrGetBsInByte(2,mp3,1);//ID3v2 version         
        Id3Version &= 0xff00;//ver and revision,now use ver
		if(Id3Version==0x200)
			Id3Version =1;
		else if((Id3Version ==0x300) ||(Id3Version ==0x400))
			Id3Version = 0;
		else//for error and future;20100825lszhang
            Id3Version = 0;


		temp[5] = Mp3PsrGetBsInByte(1,mp3,1);//ID3v2 flags             
		temp[3] = Mp3PsrGetBsInByte(1,mp3,1);
		temp[2] = Mp3PsrGetBsInByte(1,mp3,1);
		temp[1] = Mp3PsrGetBsInByte(1,mp3,1);
		temp[0] = Mp3PsrGetBsInByte(1,mp3,1);
		Id3v2len = temp[3]<<7 | temp[2];
		Id3v2len = Id3v2len<<7 | temp[1];
		Id3v2len = Id3v2len<<7 | temp[0];	
		Id3v2lenRet = Id3v2len;
		if (temp[5] & 0x10)
           Id3v2lenRet += 10;
		while((Id3v2len>=10)&&(frameinfoseg<ID3TAGNUM))
		{
			if(Id3Version == 1) //ID3V2.2
			{
				ID3FRAME.UID = Mp3PsrGetBsInByte(3,mp3,1);
				ID3FRAME.length = Mp3PsrGetBsInByte(3,mp3,1);
				//ID3FRAME.flag = Mp3PsrGetBsInByte(1,AIF);
				Id3v2len -=6;				
			}
			else
			{
				ID3FRAME.UID = Mp3PsrGetBsInByte(4,mp3,1);
				ID3FRAME.length = Mp3PsrGetBsInByte(4,mp3,1);
				ID3FRAME.flag = Mp3PsrGetBsInByte(2,mp3,1);
				Id3v2len -=10;
			}
			if(ID3FRAME.length<0)break;
			if(ID3FRAME.length==0)continue;

			if(ID3FRAME.UID == ID3UniqueTag[Id3Version][0]/* 0x54495432*/) /*TIT2*/
			{	//read tile
				frameinfoseg++;
				TxtEncodeflag=Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					Mp3PsrReadNunicode(&mp3->mtitle_sz,&mp3->mtitle,mp3,ID3FRAME.length);
					if(mp3->mInforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                mp3->mInforBufLeftLength -= (ID3FRAME.length-1);
					mp3->mInforBuf          += (ID3FRAME.length-1);			
					break;
				case 1:
					mp3->mtitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Mp3PsrReadunicode(&mp3->mtitle_sz,&mp3->mtitle,mp3,ID3FRAME.length);
					break;
				case 2:
					mp3->mtitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Mp3PsrReadunicode(&mp3->mtitle_sz,&mp3->mtitle,mp3,ID3FRAME.length);
					break;
				case 3:
					mp3->mtitleCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					Mp3PsrReadNunicode(&mp3->mtitle_sz,&mp3->mtitle,mp3,ID3FRAME.length);
					break;
				default:
					Mp3PsrSkipBsInByte(ID3FRAME.length-1,mp3);
				
				}

				Id3v2len -=ID3FRAME.length;
			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][1]/*0x54504531*/) /*TPE1*/
			{
				frameinfoseg++;
				TxtEncodeflag=Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					Mp3PsrReadNunicode(&mp3->mauthor_sz,&mp3->mauthor,mp3,ID3FRAME.length);
					if(mp3->mInforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                mp3->mInforBufLeftLength -= (ID3FRAME.length-1);
					mp3->mInforBuf          += (ID3FRAME.length-1);
					break;
				case 1:
					mp3->mauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Mp3PsrReadunicode(&mp3->mauthor_sz,&mp3->mauthor,mp3,ID3FRAME.length);
					break;
				case 2:
					mp3->mauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Mp3PsrReadunicode(&mp3->mauthor_sz,&mp3->mauthor,mp3,ID3FRAME.length);
					break;
				case 3:
					mp3->mauthorCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					Mp3PsrReadNunicode(&mp3->mauthor_sz,&mp3->mauthor,mp3,ID3FRAME.length);
					break;
				default:
					Mp3PsrSkipBsInByte(ID3FRAME.length-1,mp3);					
				}
				Id3v2len -=ID3FRAME.length;

			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][2]/*0x54414C42*/) /*TALB*/
			{
				frameinfoseg++;
				TxtEncodeflag=Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					Mp3PsrReadNunicode(&mp3->mAlbum_sz,&mp3->mAlbum,mp3,ID3FRAME.length);
					if(mp3->mInforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                mp3->mInforBufLeftLength -= (ID3FRAME.length-1);
					mp3->mInforBuf          += (ID3FRAME.length-1);
					break;
				case 1:
					mp3->mAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Mp3PsrReadunicode(&mp3->mAlbum_sz,&mp3->mAlbum,mp3,ID3FRAME.length);
					break;
				case 2:
					mp3->mAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Mp3PsrReadunicode(&mp3->mAlbum_sz,&mp3->mAlbum,mp3,ID3FRAME.length);
					break;
				case 3:
					mp3->mAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					Mp3PsrReadNunicode(&mp3->mAlbum_sz,&mp3->mAlbum,mp3,ID3FRAME.length);
					break;
				default:
					Mp3PsrSkipBsInByte(ID3FRAME.length-1,mp3);
					
				}
				Id3v2len -=ID3FRAME.length;
		}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][3]/*0x54594552*/) /*TYER*/
			{
				frameinfoseg++;
				TxtEncodeflag=Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					Mp3PsrReadNunicode(&mp3->mYear_sz,&mp3->mYear,mp3,ID3FRAME.length);
					if(mp3->mInforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                mp3->mInforBufLeftLength -= (ID3FRAME.length-1);
					mp3->mInforBuf          += (ID3FRAME.length-1);
					break;
				case 1:
					mp3->mYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Mp3PsrReadunicode(&mp3->mYear_sz,&mp3->mYear,mp3,ID3FRAME.length);
					break;
				case 2:
					mp3->mYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Mp3PsrReadunicode(&mp3->mYear_sz,&mp3->mYear,mp3,ID3FRAME.length);
					break;
				case 3:
					mp3->mYearCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					Mp3PsrReadNunicode(&mp3->mYear_sz,&mp3->mYear,mp3,ID3FRAME.length);
					break;
				default:
					Mp3PsrSkipBsInByte(ID3FRAME.length-1,mp3);
					
				}
				Id3v2len -=ID3FRAME.length;
			}

			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][4]/*0x54434f4e*/) /*TCON*/
			{
				cdx_int8 *GenreTemp = mp3->mInforBuf;//for use AIF->mInforBuf buffer
				cdx_int32 GenreLen,ReadLen=10;
				cdx_uint32 GenreIndex;

				frameinfoseg++;
				TxtEncodeflag=Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				GenreLen = ID3FRAME.length-1;
				Id3v2len -=ID3FRAME.length;
					ReadLen = GenreLen;
				if(ReadLen>mp3->mInforBufLeftLength)
				{
					ReadLen = mp3->mInforBufLeftLength;
				}

				if(ReadLen == 0)
				{	
					continue;
				}
				for(i=0;i<ReadLen;i++)
				{
					data= (char )Mp3PsrGetBsInByte(1,mp3,1);
					GenreTemp[i]= data;
				}				
				GenreLen -= ReadLen;
				Mp3PsrSkipBsInByte(GenreLen,mp3);

				if(GenreTemp[0]=='(')
				{
					switch(TxtEncodeflag)
					{
					case 0:
						break;
					case 1:
					case 3:
						for(i=0;i<((ReadLen-2)/2);i++)
						{
							GenreTemp[i]= GenreTemp[2*i+2];//skip BOM
						}
						ReadLen = (ReadLen-2)/2;
						break;
						
					case 2:
						for(i=0;i<((ReadLen-2)/2);i++)
						{
							GenreTemp[i]= GenreTemp[2*i+1+2];//skip BOM
						}
						ReadLen = (ReadLen-2)/2;
						break;
						
					}
					
					if((GenreTemp[1]=='R')&&(GenreTemp[2]=='X'))
						GenreIndex = 148;
					else if((GenreTemp[1]=='C')&&(GenreTemp[2]=='R'))
						GenreIndex = 149;
					else
					{	
						i = 1;
						GenreIndex = 0;
						while((i<ReadLen)&&(GenreIndex<148+2))
						{	
							if(GenreTemp[i]==')')
								break;
							GenreIndex *=10;
							GenreIndex += (int)((GenreTemp[i]-0x30)&0xff);
							i++;
						}
						if(GenreIndex<148+2)
						{
							mp3->mGenreCharEncode = A_AUDIO_FONTTYPE_ISOIEC8859_1;
							mp3->mGenre_sz = 22;
							mp3->mGenre = mp3->mInforBuf;
							if(mp3->mInforBufLeftLength<22)
								return 0;
							mp3->mInforBufLeftLength -=22;			
							
							memcpy(mp3->mGenre,ID3V1GENRE[GenreIndex],22);
							mp3->mGenre_sz=Mp3PsrGetStringLen((cdx_int8*)mp3->mGenre);
							mp3->mInforBuf += 22;
						}
						
					}
				}else{
					mp3->mGenreCharEncode = (cdx_audio_fonttype_e)TxtEncodeflag;
					mp3->mGenre_sz =  ReadLen;	
					mp3->mGenre = mp3->mInforBuf;
					i = 0;
					if((TxtEncodeflag==1)||(TxtEncodeflag==2))
					{
						i=2;
						mp3->mGenre_sz -=2;
					}
					mp3->mInforBufLeftLength -= ReadLen - i;
					for(;i<ReadLen;i++)
					{
						*mp3->mInforBuf++ = GenreTemp[i];
					}
				}

			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][5]/*0x41504943*/) /*APIC*/
			{
				cdx_int8 *p = 0;
				cdx_int8 *GenreTemp = mp3->mInforBuf;//for use AIF->mInforBuf buffer
				cdx_id3_image_info_t image;
				frameinfoseg++;
				TxtEncodeflag = Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				Id3v2len -=ID3FRAME.length;
				ID3FRAME.length -= 1;
				memset(&image,0,sizeof(cdx_id3_image_info_t));
				if(ID3FRAME.length == 0)continue;
				    
				i = 0;
				do
				{
					data= (char )Mp3PsrGetBsInByte(1,mp3,1);
					GenreTemp[i]= data;
					i++;
				}while(data != 0);
				p = (cdx_int8*)GenreTemp;
				if(!strcmp(p, IMAGE_MIME_TYPE_BMP))
				{
				   image.img_format = IMG_FORMAT_BMP;
				}
				else if(!strcmp(p, IMAGE_MIME_TYPE_JPEG))
				{
				   image.img_format = IMG_FORMAT_JPG;
				}
				else if(!strcmp(p, IMAGE_MIME_TYPE_PNG))
				{
				   image.img_format = IMG_FORMAT_PNG;
				}
				else
				{
				   image.img_format = IMG_FORMAT_UNSUPPORT;
				}
				ID3FRAME.length -=  i;
				if(ID3FRAME.length == 0)continue;
				    image.pic_type = Mp3PsrGetBsInByte(1,mp3,1);
				ID3FRAME.length -= 1;				
				if(ID3FRAME.length == 0)continue;
				    
				i = 0;
				do
				{
					data= (cdx_int8)Mp3PsrGetBsInByte(1,mp3,1);
					if((TxtEncodeflag ==1)||(TxtEncodeflag ==2))
					{
						data |= (cdx_int8)Mp3PsrGetBsInByte(1,mp3,1);
						i++;
					}
					i++;
				}while(data != 0);

				switch(TxtEncodeflag)
				{
				case 0:
					
					break;
				case 1:
					mp3->mAPicCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					break;
				case 2:
					mp3->mAPicCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					break;
				case 3:
					mp3->mAPicCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					
					break;
				default:
					;
					
				}
				ID3FRAME.length -= i;
				if(ID3FRAME.length == 0) continue;
				mp3->mAPic_sz = sizeof(cdx_id3_image_info_t);
				CDX_LOGE("hehehehe1  U should load albumart 'APIC' here   lenth:%d",ID3FRAME.length);
				image.length = ID3FRAME.length;

				mp3->mAlbumArtBufSize = image.length;
				if(mp3->pAlbumArtBuf != NULL)
					free(mp3->pAlbumArtBuf);
				mp3->pAlbumArtBuf = (cdx_uint8*)malloc(image.length);
				if(mp3->pAlbumArtBuf == NULL)
				{
					loge("malloc failed!");
					return -1;
				}
				
				image.FileLocation = CdxStreamTell(mp3->stream);
				CdxStreamRead(mp3->stream,mp3->pAlbumArtBuf,image.length);
				CdxStreamSeek(mp3->stream,image.FileLocation,SEEK_SET);
				
				if(image.FileLocation < 0)image.FileLocation = 0;
				mp3->mInforBufLeftLength -= 4-((intptr_t)(mp3->mInforBuf)&0x3);
				mp3->mInforBuf += 4-((intptr_t)(mp3->mInforBuf)&0x3);
				mp3->mAPic = (cdx_id3_image_info_t*)(mp3->mInforBuf);
				if(mp3->mInforBufLeftLength < mp3->mAPic_sz)
					break;				
				mp3->mInforBufLeftLength -= mp3->mAPic_sz;
				memcpy(mp3->mAPic,&image,mp3->mAPic_sz);
				mp3->mInforBuf += mp3->mAPic_sz;
				Mp3PsrSkipBsInByte(ID3FRAME.length,mp3);
			}
			else if((ID3FRAME.UID == ID3UniqueTag[Id3Version][6]/*0x434f4d4d*/) /*COMM*/
				    ||(ID3FRAME.UID == ID3UniqueTag[Id3Version][6]/*0x544c414e*/) /*TLAN*/)
			{
				cdx_int8 ulcomm[4] ={0,0,0,0} ;
				

				frameinfoseg++;
				TxtEncodeflag=Mp3PsrGetBsInByte(1,mp3,1);//TEXT ENCODERing 00
				ulcomm[0] = Mp3PsrGetBsInByte(1,mp3,1);
				ulcomm[1] = Mp3PsrGetBsInByte(1,mp3,1);
				ulcomm[2] = Mp3PsrGetBsInByte(1,mp3,1);
				ulLanguage_encoding = Mp3PsrGetLanguageEncoding(ulcomm);
				Mp3PsrSkipBsInByte(ID3FRAME.length-1-3,mp3);
				Id3v2len -=ID3FRAME.length;
			}
			else
			{
				if(Id3v2len<ID3FRAME.length)
					ID3FRAME.length = Id3v2len;
				Mp3PsrSkipBsInByte(ID3FRAME.length,mp3);
				Id3v2len -=ID3FRAME.length;
			}
      
		} 
		  
		Mp3PsrSkipBsInByte(Id3v2len,mp3);
		Id3v2lenRet +=10;
	} 
	if(ulLanguage_encoding==A_AUDIO_FONTTYPE_WINDOWS_1251)
	{
			Mp3PsrEncodingChange(mp3);
	}
	//for other ID3
	BsVal = Mp3PsrGetBsInByte(4,mp3,0);
	BsVal >>=8;
	while(BsVal == 0x494433/*ID3*/) 
	{
		cdx_uint8 *p = (cdx_uint8*)temp;
		for(i=0;i<10;i++)
		{
			*p++ = Mp3PsrShowBs(i,1,mp3);
		}
		i =  Mp3PsrGETID3Len((cdx_uint8*)temp,10);
		if(i<0)
		{
			break;
		}
		Mp3PsrSkipBsInByte(i,mp3);
		Id3v2lenRet +=i;
		BsVal = Mp3PsrGetBsInByte(4,mp3,0);
		BsVal >>=8;
	}
      
	return	Id3v2lenRet;
}     


cdx_bool GetMPEGAudioFrameSize(
        uint32_t header, size_t *frameSize,
        int *outSamplingRate, int *outChannels,
        int *outBitrate, int *outNumSamples) ;


static cdx_int32 Resync(CdxStreamProbeDataT *p){
	//const sp<DataSource> &source, uint32_t match_header,
	//off64_t *inout_pos, off64_t *post_id3_pos, uint32_t *out_header) {
	cdx_int64  inout_pos,post_id3_pos;
	cdx_uint32 out_header,match_header;
 	cdx_uint8 *source = (cdx_uint8 *)p->buf;
	
	post_id3_pos = 0;
    inout_pos = 0;
	match_header = 0;
    cdx_int64 pos1 = 0;
	
    if (inout_pos == 0) {
        // Skip an optional ID3 header if syncing at the very beginning
        // of the datasource.

        for (;;) {
            cdx_uint8 id3header[10];
            pos1 += sizeof(id3header);
            if(pos1 > p->len)
            {
                return 0;
            }
			memcpy(id3header,source + inout_pos,sizeof(id3header));
            //if (source->readAt(*inout_pos, id3header, sizeof(id3header))
              //      < (ssize_t)sizeof(id3header)) {
                // If we can't even read these 10 bytes, we might as well bail
                // out, even if there _were_ 10 bytes of valid mp3 audio data...
                //return false;
            //}

            if (memcmp("ID3", id3header, 3)) {
                break;
            }

            // Skip the ID3v2 header.

            cdx_int32 len =
                ((id3header[6] & 0x7f) << 21)
                | ((id3header[7] & 0x7f) << 14)
                | ((id3header[8] & 0x7f) << 7)
                | (id3header[9] & 0x7f);

            len += 10;

            inout_pos += len;

            CDX_LOGE("skipped ID3 tag, new starting offset is %lld (0x%016llx)",
                 inout_pos, inout_pos);
        }
		post_id3_pos = inout_pos;
       
    }

    cdx_int64 pos = inout_pos;
    cdx_int32  valid = 0;

    //const cdx_int32 kMaxReadBytes = 1024;
    //const cdx_int32 kMaxBytesChecked = 128 * 1024;
    cdx_uint8 buf[kMaxReadBytes];
    cdx_int32 bytesToRead = kMaxReadBytes;
    cdx_int32 totalBytesRead = 0;
    cdx_int32 remainingBytes = 0;
    cdx_int32 reachEOS = 0;
    cdx_uint8 *tmp = buf;

    do {
        if (pos >= inout_pos + kMaxBytesChecked) {
            // Don't scan forever.
            CDX_LOGW("giving up at offset %lld", pos);
            break;
        }

        if (remainingBytes < 4) {
            if (reachEOS) {
                break;
            } else {
                memcpy(buf, tmp, remainingBytes);
                bytesToRead = kMaxReadBytes - remainingBytes;

                /*
                 * The next read position should start from the end of
                 * the last buffer, and thus should include the remaining
                 * bytes in the buffer.
                 */
                pos1 += bytesToRead;
                if(pos1 > p->len)
                {
                    return 0;
                }
                memcpy(buf + remainingBytes,source + pos + remainingBytes,bytesToRead);
				totalBytesRead = bytesToRead;
                //totalBytesRead = source->readAt(pos + remainingBytes,
                                               // buf + remainingBytes,
                                               // bytesToRead);
                //if (totalBytesRead <= 0) {
                 //   break;
                //}
                reachEOS = (totalBytesRead != bytesToRead);
                totalBytesRead += remainingBytes;
                remainingBytes = totalBytesRead;
                tmp = buf;
                continue;
            }
        }

        cdx_uint32 header = AV_RB32(tmp);

        if (match_header != 0 && (header & kMask) != (match_header & kMask)) {
            ++pos;
            ++tmp;
            --remainingBytes;
            continue;
        }

        size_t frame_size;
        cdx_int32  sample_rate,num_channels, bitrate, NumSamples;
        if (!GetMPEGAudioFrameSize(
                    header, &frame_size,
                    &sample_rate, &num_channels, &bitrate, &NumSamples)) {
            ++pos;
            ++tmp;
            --remainingBytes;
            continue;
        }

        CDX_LOGV("found possible 1st frame at %lld (header = 0x%08x)", pos, header);

        // We found what looks like a valid frame,
        // now find its successors.

        cdx_int64 test_pos = pos + frame_size;

        valid = 1;
		cdx_int32 j = 0;
        for (j = 0; j < 3; ++j) {
            cdx_uint8 tmp[4];
            pos1 += 4;
            if(pos1 > p->len)
            {
                return 0;
            }
			memcpy(tmp, source + test_pos, 4);
            //if (source->readAt(test_pos, tmp, 4) < 4) {
               // valid = false;
               // break;
            //}

            cdx_uint32 test_header = AV_RB32(tmp);

            CDX_LOGV("subsequent header is %08x", test_header);

            if ((test_header & kMask) != (header & kMask)) {
                valid = 0;
                break;
            }

            size_t test_frame_size;
            if (!GetMPEGAudioFrameSize(
                        test_header, &test_frame_size,
                        &sample_rate, &num_channels, &bitrate, &NumSamples)) {
                valid = 0;
                break;
            }

            CDX_LOGV("found subsequent frame #%d at %lld", j + 2, test_pos);

            test_pos += test_frame_size;
        }

        if (valid) {
            inout_pos = pos;
			out_header = header;
            
        } else {
            CDX_LOGE("no dice, no valid sequence of frames found.");
        }

        ++pos;
        ++tmp;
        --remainingBytes;
    } while (!valid);

    return valid;
}


static inline int FFMpaCheckHeader(cdx_uint32 header){
    /* header */
    if ((header & 0xffe00000) != 0xffe00000)
        return -1;
    /* layer check */
    if ((header & (3<<17)) == 0)
        return -1;
    /* bit rate */
    if ((header & (0xf<<12)) == 0xf<<12)
        return -1;
    /* frequency */
    if ((header & (3<<10)) == 3<<10)
        return -1;
    return 0;
}

#if 0
static int FFMpegaudioDecodeHeader(MPADecodeHeader *s, cdx_uint32 header)
{
    int mSampleRate, mFrameSize, mpeg25, padding;
    int mSampleRateIndex, mBitrateIndex;
    if (header & (1<<20)) {
        s->lsf = (header & (1<<19)) ? 0 : 1;
        mpeg25 = 0;
    } else {
        s->lsf = 1;
        mpeg25 = 1;
    }

    s->layer = 4 - ((header >> 17) & 3);
    /* extract frequency */
    mSampleRateIndex = (header >> 10) & 3;
    mSampleRate = FFMpaFreqTab[mSampleRateIndex] >> (s->lsf + mpeg25);
    mSampleRateIndex += 3 * (s->lsf + mpeg25);
    s->mSampleRateIndex = mSampleRateIndex;
    s->errorProtection = ((header >> 16) & 1) ^ 1;
    s->mSampleRate = mSampleRate;

    mBitrateIndex = (header >> 12) & 0xf;
    padding = (header >> 9) & 1;
    //extension = (header >> 8) & 1;
    s->mode = (header >> 6) & 3;
    s->modeExt = (header >> 4) & 3;
    //copyright = (header >> 3) & 1;
    //original = (header >> 2) & 1;
    //emphasis = header & 3;

    if (s->mode == MPA_MONO)
        s->nChannels = 1;
    else
        s->nChannels = 2;

    if (mBitrateIndex != 0) {
        mFrameSize = FFMpaBitrateTab[s->lsf][s->layer - 1][mBitrateIndex];
        s->mBitRate = mFrameSize * 1000;
        switch(s->layer) {
        case 1:
            mFrameSize = (mFrameSize * 12000) / mSampleRate;
            mFrameSize = (mFrameSize + padding) * 4;
            break;
        case 2:
            mFrameSize = (mFrameSize * 144000) / mSampleRate;
            mFrameSize += padding;
            break;
        default:
        case 3:
            mFrameSize = (mFrameSize * 144000) / (mSampleRate << s->lsf);
            mFrameSize += padding;
            break;
        }
        s->mFrameSize = mFrameSize;
    } else {
        /* if no frame size computed, signal it */
        return 1;
    }

    return 0;
}

static int FFMpaDecodeHeader(cdx_uint32 head, int *mSampleRate, int *nChannels, int *frameSize, int *mBitRate,int *layer)
{
    MPADecodeHeader s1, *s = &s1;

    if (FFMpaCheckHeader(head) != 0)
        return -1;

    if (FFMpegaudioDecodeHeader(s, head) != 0) {
        return -1;
    }

    switch(s->layer) {
    case 1:
        //avctx->codec_id = CODEC_ID_MP1;
        *frameSize = 384;
        break;
    case 2:
        //avctx->codec_id = CODEC_ID_MP2;
        *frameSize = 1152;
        break;
    default:
    case 3:
        //avctx->codec_id = CODEC_ID_MP3;
        if (s->lsf)
            *frameSize = 576;
        else
            *frameSize = 1152;
        break;
    }

    *mSampleRate = s->mSampleRate;
    *nChannels = s->nChannels;
    *mBitRate = s->mBitRate;
    *layer = s->layer;
    return s->mFrameSize;
}
#endif
#if 0
static int FFMp3Probe(CdxStreamProbeDataT *p)
{
    int mMaxFrames, mFirstFrames = 0;
    int fSize, mFrames, mSampleRate;
    cdx_uint32 nHeader;
    cdx_uint8 *buf, *buf0, *buf2, *end;
    int layer = 0;

    buf0 = (uint8_t *)p->buf;
    if(FFId3v2Match(buf0)) {
        buf0 += FFId3v2TagLen(buf0);
    }
    end = (uint8_t *)p->buf + p->len - sizeof(cdx_uint32);
    while(buf0 < end && !*buf0)
        buf0++;

    mMaxFrames = 0;
    buf = buf0;

		
    for(; buf < end; buf= buf2 + 1) {
        buf2 = buf;
        if (FFMpaCheckHeader(AV_RB32(buf2)))
            continue;
        
        for(mFrames = 0; buf2 < end; mFrames++) {
            nHeader = AV_RB32(buf2);
            fSize = FFMpaDecodeHeader(nHeader, &mSampleRate, &mSampleRate, &mSampleRate, &mSampleRate,&layer);
            if(fSize < 0){
				return 0;//break;//for error check aac->mp3;
			}
				
            buf2 += fSize;
        }
        mMaxFrames = FFMAX(mMaxFrames, mFrames);
        if(buf == buf0)
            mFirstFrames= mFrames;
    }

    // keep this in sync with ac3 probe, both need to avoid
    // issues with MPEG-files!
    if((layer == 1)||(layer == 2))
        return AVPROBE_SCORE_MAX -3;
    if (mFirstFrames >= 4)    
        return AVPROBE_SCORE_MAX / 2 + 1;
    else if(mMaxFrames > 500) 
        return AVPROBE_SCORE_MAX / 2;
    else if(mMaxFrames >= 2) 
        return AVPROBE_SCORE_MAX / 4;
    else if(buf0 != (uint8_t *)p->buf)
        return AVPROBE_SCORE_MAX / 4 - 1;
    else 
        return 0;
}
#endif

cdx_bool GetMPEGAudioFrameSize(
        uint32_t header, size_t *frameSize,
        int *outSamplingRate, int *outChannels,
        int *outBitrate, int *outNumSamples) {
    *frameSize = 0;

    if (outSamplingRate) {
        *outSamplingRate = 0;
    }

    if (outChannels) {
        *outChannels = 0;
    }

    if (outBitrate) {
        *outBitrate = 0;
    }

    if (outNumSamples) {
        *outNumSamples = 1152;
    }

    if ((header & 0xffe00000) != 0xffe00000) {
        return CDX_FALSE;
    }

    unsigned version = (header >> 19) & 3;

    if (version == 0x01) {
        return CDX_FALSE;
    }

    unsigned layer = (header >> 17) & 3;

    if (layer == 0x00) {
        return CDX_FALSE;
    }

    //unsigned protection = (header >> 16) & 1;

    unsigned bitrateIndex = (header >> 12) & 0x0f;

    if (bitrateIndex == 0 || bitrateIndex == 0x0f) {
        // Disallow "free" bitrate.
        return CDX_FALSE;
    }

    unsigned sampling_rate_index = (header >> 10) & 3;

    if (sampling_rate_index == 3) {
        return CDX_FALSE;
    }

    static const int kSamplingRateV1[] = { 44100, 48000, 32000 };
    int samplingRate = kSamplingRateV1[sampling_rate_index];
    if (version == 2 /* V2 */) {
        samplingRate /= 2;
    } else if (version == 0 /* V2.5 */) {
        samplingRate /= 4;
    }

    unsigned padding = (header >> 9) & 1;

    if (layer == 3) {
        // layer I

        static const int kBitrateV1[] = {
            32, 64, 96, 128, 160, 192, 224, 256,
            288, 320, 352, 384, 416, 448
        };

        static const int kBitrateV2[] = {
            32, 48, 56, 64, 80, 96, 112, 128,
            144, 160, 176, 192, 224, 256
        };

        int bitRate =
            (version == 3 /* V1 */)
                ? kBitrateV1[bitrateIndex - 1]
                : kBitrateV2[bitrateIndex - 1];

        if (outBitrate) {
            *outBitrate = bitRate;
        }

        *frameSize = (12000 * bitRate / samplingRate + padding) * 4;

        if (outNumSamples) {
            *outNumSamples = 384;
        }
    } else {
        // layer II or III

        static const int kBitrateV1L2[] = {
            32, 48, 56, 64, 80, 96, 112, 128,
            160, 192, 224, 256, 320, 384
        };

        static const int kBitrateV1L3[] = {
            32, 40, 48, 56, 64, 80, 96, 112,
            128, 160, 192, 224, 256, 320
        };

        static const int kBitrateV2[] = {
            8, 16, 24, 32, 40, 48, 56, 64,
            80, 96, 112, 128, 144, 160
        };

        int bitRate;
        if (version == 3 /* V1 */) {
            bitRate = (layer == 2 /* L2 */)
                ? kBitrateV1L2[bitrateIndex - 1]
                : kBitrateV1L3[bitrateIndex - 1];

            if (outNumSamples) {
                *outNumSamples = 1152;
            }
        } else {
            // V2 (or 2.5)

            bitRate = kBitrateV2[bitrateIndex - 1];
            if (outNumSamples) {
                *outNumSamples = (layer == 1 /* L3 */) ? 576 : 1152;
            }
        }

        if (outBitrate) {
            *outBitrate = bitRate;
        }

        if (version == 3 /* V1 */) {
            *frameSize = 144000 * bitRate / samplingRate + padding;
        } else {
            // V2 or V2.5
            size_t tmp = (layer == 1 /* L3 */) ? 72000 : 144000;
            *frameSize = tmp * bitRate / samplingRate + padding;
        }
    }

    if (outSamplingRate) {
        *outSamplingRate = samplingRate;
    }

    if (outChannels) {
        int channelMode = (header >> 6) & 3;

        *outChannels = (channelMode == 3) ? 1 : 2;
    }

    return CDX_TRUE;
}

static XINGSeeker *CreateXINGSeeker(CdxParserT *parser,  cdx_int64 firstFramePos)
{
    MP3ParserImpl *mp3;
    cdx_int32     noSeeker = 0;    
    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser lib has not been initiated!");
        return NULL;
    }
    
    XINGSeeker *seeker = (XINGSeeker *)malloc (sizeof(XINGSeeker));

    seeker->mFirstFramePos = firstFramePos;

    uint8_t buffer[4];
    int offset = firstFramePos;
    if (CdxStreamSeek(mp3->stream, offset, SEEK_SET)) {
         noSeeker = 1;
         goto EXIT;    
    }
    if (CdxStreamRead(mp3->stream, &buffer, 4) < 4) { // get header
        noSeeker = 1;
        goto EXIT;
    }
    offset += 4;

    int header = U32_AT(buffer);;
    size_t xingFramesize = 0;
    int samplingRate = 0;
    int mNumChannels;
    int mSamplePerFrame = 0;
    if (!GetMPEGAudioFrameSize(header, &xingFramesize, &samplingRate, &mNumChannels,
                               NULL, &mSamplePerFrame)) {
        noSeeker = 1;
        goto EXIT;
    }
    seeker->mFirstFramePos += xingFramesize;

    uint8_t version = (buffer[1] >> 3) & 3;

    // determine offset of XING header
    if(version & 1) { // mpeg1
        if (mNumChannels != 1) offset += 32;
        else offset += 17;
    } else { // mpeg 2 or 2.5
        if (mNumChannels != 1) offset += 17;
        else offset += 9;
    }

    int xingbase = offset;
    if (CdxStreamSeek(mp3->stream, offset, SEEK_SET)) {
         noSeeker = 1;
         goto EXIT;
    }
    if (CdxStreamRead(mp3->stream, &buffer, 4) < 4) { // XING header ID
        noSeeker = 1;
        goto EXIT;
    }
    offset += 4;

    // Check XING ID
    if ((buffer[0] != 'X') || (buffer[1] != 'i')
                || (buffer[2] != 'n') || (buffer[3] != 'g')) {
        if ((buffer[0] != 'I') || (buffer[1] != 'n')
                    || (buffer[2] != 'f') || (buffer[3] != 'o')) {
            noSeeker = 1;
            goto EXIT;
        }
    }
    if (CdxStreamSeek(mp3->stream, offset, SEEK_SET)) {
         noSeeker = 1;
         goto EXIT; 
    }
    if (CdxStreamRead(mp3->stream, &buffer, 4) < 4) { // flags
        noSeeker = 1;
        goto EXIT;
    }
    offset += 4;
    uint32_t flags = U32_AT(buffer);

    if (flags & 0x0001) {  // Frames field is present
        if (CdxStreamSeek(mp3->stream, offset, SEEK_SET)) {
             noSeeker = 1;
             goto EXIT; 
        }
        if (CdxStreamRead(mp3->stream, buffer, 4) < 4) {
            noSeeker = 1;
            goto EXIT;
        }
        int32_t frames = U32_AT(buffer);
        // only update mDurationUs if the calculated duration is valid (non zero)
        // otherwise, leave duration at -1 so that getDuration() and getOffsetForTime()
        // return false when called, to indicate that this xing tag does not have the
        // requested information
        if (frames) {
            seeker->mDurationUs = (int64_t)frames * mSamplePerFrame * 1000000LL / samplingRate;
        }
        offset += 4;
    }
    if (flags & 0x0002) {  // Bytes field is present
        if (CdxStreamSeek(mp3->stream, offset, SEEK_SET)) {
            noSeeker = 1;
            goto EXIT; 
        }
        if (CdxStreamRead(mp3->stream, buffer, 4) < 4) {
            noSeeker = 1;
            goto EXIT;
        }
        seeker->mSizeBytes = U32_AT(buffer);
        offset += 4;
    }
    if (flags & 0x0004) {  // TOC field is present
        if (CdxStreamSeek(mp3->stream, offset, SEEK_SET)) {
            noSeeker = 1;
            goto EXIT; 
        }
        if (CdxStreamRead(mp3->stream, seeker->mTOC, 99) < 99) {
            noSeeker = 1;
            goto EXIT;
        }
        seeker->mTOCValid = CDX_TRUE;
        offset += 100;
    }

    if (CdxStreamSeek(mp3->stream, xingbase + 0xb1 - 0x24, SEEK_SET)) {
         noSeeker = 1;
         goto EXIT; 
    }

    if (CdxStreamRead(mp3->stream, &buffer, 3) == 3) {
        seeker->mEncoderDelay = (buffer[0] << 4) + (buffer[1] >> 4);
        seeker->mEncoderPadding = ((buffer[1] & 0xf) << 8) + buffer[2];
    }

EXIT:
    if (noSeeker) {
        free(seeker);
        seeker = NULL;
    }
    return seeker;        
}

cdx_bool GetXINGOffsetForTime(XINGSeeker *seeker, int64_t *timeUs, int64_t *pos) {
    if (seeker == NULL ||seeker->mSizeBytes == 0 || !seeker->mTOCValid || seeker->mDurationUs < 0) {
        return CDX_FALSE;
    }

    float percent = (float)(*timeUs) * 100 / seeker->mDurationUs;
    float fx;
    if( percent <= 0.0f ) {
        fx = 0.0f;
    } else if( percent >= 100.0f ) {
        fx = 256.0f;
    } else {
        int a = (int)percent;
        float fa, fb;
        if ( a == 0 ) {
            fa = 0.0f;
        } else {
            fa = (float)seeker->mTOC[a-1];
        }
        if ( a < 99 ) {
            fb = (float)seeker->mTOC[a];
        } else {
            fb = 256.0f;
        }
        fx = fa + (fb-fa)*(percent-a);
    }

    *pos = (int)((1.0f/256.0f)*fx*seeker->mSizeBytes) + seeker->mFirstFramePos;
    
    CDX_LOGV("GetXINGOffsetForTime %lld us => %08lld", *timeUs, *pos);

    return CDX_TRUE;
}

static VBRISeeker *CreateVBRISeeker(CdxParserT *parser,  cdx_int64 postId3Pos)
{
    MP3ParserImpl *mp3;
    cdx_int32     noSeeker = 0;
    VBRISeeker *seeker  = NULL;
        
    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser lib has not been initiated!");
        return NULL;
    }

    cdx_int64 pos = postId3Pos;
    if (CdxStreamSeek(mp3->stream, pos, SEEK_SET))
         return NULL;

    uint8_t header[4];
    ssize_t n = CdxStreamRead(mp3->stream, header, sizeof(header));
    if (n < (ssize_t)sizeof(header)) {
        return NULL;
    }

    uint32_t tmp = U32_AT(&header[0]);
    size_t frameSize;
    int sampleRate;
    if (!GetMPEGAudioFrameSize(tmp, &frameSize, &sampleRate, NULL, NULL, NULL)) {
        return NULL;
    }

    // VBRI header follows 32 bytes after the header _ends_.
    pos += sizeof(header) + 32;

    uint8_t vbriHeader[26];
    if (CdxStreamSeek(mp3->stream, pos, SEEK_SET))
        return NULL;
    n = CdxStreamRead(mp3->stream, vbriHeader, sizeof(vbriHeader));
    if (n < (ssize_t)sizeof(vbriHeader)) {
        return NULL;
    }

    if (memcmp(vbriHeader, "VBRI", 4)) {
        return NULL;
    }

    size_t numFrames = U32_AT(&vbriHeader[14]);

    int64_t durationUs =
        numFrames * 1000000ll * (sampleRate >= 32000 ? 1152 : 576) / sampleRate;

    CDX_LOGV("duration = %.2f secs", durationUs / 1E6);

    size_t numEntries = U16_AT(&vbriHeader[18]);
    size_t entrySize = U16_AT(&vbriHeader[22]);
    size_t scale = U16_AT(&vbriHeader[20]);

    CDX_LOGV("%d entries, scale=%d, size_per_entry=%d",
         numEntries,
         scale,
         entrySize);

    size_t totalEntrySize = numEntries * entrySize;
    uint8_t *buffer = (uint8_t *)malloc(totalEntrySize);
    
    if (CdxStreamSeek(mp3->stream, pos, SEEK_SET)) {
        noSeeker = 1;
        goto EXIT;
    }
    n = CdxStreamRead(mp3->stream, buffer, totalEntrySize);
    if (n < (ssize_t)totalEntrySize) {
        noSeeker = 1;
        goto EXIT;
    }

    seeker = (VBRISeeker *)malloc(sizeof(VBRISeeker));
    seeker->mBasePos = postId3Pos + frameSize;
    // only update mDurationUs if the calculated duration is valid (non zero)
    // otherwise, leave duration at -1 so that getDuration() and getOffsetForTime()
    // return false when called, to indicate that this vbri tag does not have the
    // requested information
    if (durationUs) {
        seeker->mDurationUs = durationUs;
    }

/*    seeker->mSegments = (cdx_uint32 *)malloc(numEntries);*/
/*    if (!seeker->mSegments) {*/
/*        noSeeker = 1;*/
/*        goto EXIT;*/
/*    }*/
	memset(&seeker->mSegments, 0, sizeof(seeker->mSegments));
    cdx_int64 offset = postId3Pos;
    size_t i;

    for (i = 0; i < numEntries; i++) {
        uint32_t numBytes;
        switch (entrySize) {
            case 1: numBytes = buffer[i]; break;
            case 2: numBytes = U16_AT(buffer + 2 * i); break;
            case 3: numBytes = U24_AT(buffer + 3 * i); break;
            default:
            {
                numBytes = U32_AT(buffer + 4 * i); break;
            }
        }

        numBytes *= scale;
        
        seeker->mSegments[i] = numBytes;
        seeker->mSegmentsize++;
        
        CDX_LOGV("entry #%d: %d offset %08lld", i, numBytes, offset);
        offset += numBytes;
    }

    CDX_LOGI("Found VBRI header.");
EXIT:
    if (buffer) {
        free(buffer);
        buffer = NULL;
    }
    if (noSeeker) {
        if (seeker) {
/*            if (seeker->mSegments) {*/
/*                free(seeker->mSegments);*/
/*                seeker->mSegments = NULL;*/
/*            }*/
            free(seeker);
            seeker = NULL;
        }
    }
    return seeker;
}

static cdx_bool GetVBRIOffsetForTime(VBRISeeker *seeker, int64_t *timeUs, int64_t *pos) {
    if (seeker == NULL || seeker->mDurationUs < 0) {
        return CDX_FALSE;
    }

    int64_t segmentDurationUs = seeker->mDurationUs / seeker->mSegmentsize;
    int64_t nowUs = 0;
    *pos = seeker->mBasePos;
    size_t segmentIndex = 0;
    while (segmentIndex < seeker->mSegmentsize && nowUs < *timeUs) {
        nowUs += segmentDurationUs;
        *pos += seeker->mSegments[segmentIndex];
    }

    CDX_LOGV("GetVBRIOffsetForTime %lld us => 0x%08lld", *timeUs, *pos);

    *timeUs = nowUs;

    return CDX_TRUE;
}

static cdx_bool Mp3HeadResync(CdxParserT *parser, cdx_uint32 matchHeader, cdx_int64 *inoutPos, cdx_int64 *postId3Pos, cdx_int32 *outHead)
{
    MP3ParserImpl *mp3;
    cdx_bool   ret = CDX_FALSE;
    size_t mFrameSize;
    int mSampleRate, mNumChannels, mBitRate;

    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser lib has not been initiated!");
        return ret;
    }

    if (*inoutPos == 0) {  
        for (;;) {
            uint8_t id3header[10];
            if (CdxStreamRead(mp3->stream, id3header, sizeof(id3header))
                        < (cdx_int32)sizeof(id3header)) {
                 return ret;
                }

            if (memcmp("ID3", id3header, 3)) {
                break;
            }

           size_t len =
                ((id3header[6] & 0x7f) << 21)
                | ((id3header[7] & 0x7f) << 14)
                | ((id3header[8] & 0x7f) << 7)
                | (id3header[9] & 0x7f);     

            len += 10;

            *inoutPos += len;   

            if (postId3Pos != NULL) {
                *postId3Pos = *inoutPos;
            }
         
        }
    }
    
    cdx_int64 pos = *inoutPos;
    if (CdxStreamSeek(mp3->stream, pos, SEEK_SET))
         return ret;
    
    CDX_LOGV("pos %lld", pos);
    cdx_bool valid = CDX_FALSE;
    cdx_uint8 buf[kMaxReadBytes];
    int bytesToRead = kMaxReadBytes;
    int totalBytesRead = 0;
    int remainingBytes = 0;
    cdx_bool reachEOS = CDX_FALSE;
    cdx_uint8 *tmp = buf;

    do {
        if (pos >= *inoutPos + kMaxBytesChecked) {
            // Don't scan forever.
            CDX_LOGV("giving up at offset %lld", pos);
            break;
        }
        
        if (remainingBytes < 4) {
            if (reachEOS) {
                break;
            } else {
                memcpy(buf, tmp, remainingBytes);
                bytesToRead = kMaxReadBytes - remainingBytes;

                totalBytesRead = CdxStreamRead(mp3->stream,
                                                buf + remainingBytes,
                                                bytesToRead);
                if (totalBytesRead <= 0) {
                    break;
                }
                reachEOS = (totalBytesRead != bytesToRead);
                totalBytesRead += remainingBytes;
                remainingBytes = totalBytesRead;
                tmp = buf;
                continue;
            }
        }
        cdx_uint32 header = U32_AT(tmp);
        
        if (matchHeader != 0 && (header & kMask) != (matchHeader & kMask)) {
            ++pos;
            ++tmp;
            --remainingBytes;
            continue;
        }       
        if (!GetMPEGAudioFrameSize(
                    header, &mFrameSize,
                    &mSampleRate, &mNumChannels, &mBitRate, NULL)) {
            ++pos;
            ++tmp;
            --remainingBytes;
            continue;
        }

        CDX_LOGV("found possible 1st frame at %lld (header = 0x%08x) mFrameSize %d", pos, header, mFrameSize);

        // We found what looks like a valid frame,
        // now find its successors.
        cdx_int64 test_pos = pos + mFrameSize;

        valid = CDX_TRUE;
        int j;
        for (j = 0; j < 3; ++j) {
            uint8_t tmp[4];
            if (CdxStreamSeek(mp3->stream, test_pos, SEEK_SET)) {
                valid = CDX_FALSE;
                break;
            }
            if (CdxStreamRead(mp3->stream, tmp, 4) < 4) {
                valid = CDX_FALSE;
                break;
            }

            uint32_t test_header = U32_AT(tmp);

            CDX_LOGV("subsequent header is %08x", test_header);

            if ((test_header & kMask) != (header & kMask)) {
                valid = CDX_FALSE;
                break;
            }

            size_t test_frame_size;
            if (!GetMPEGAudioFrameSize(
                        test_header, &test_frame_size, NULL, NULL, NULL, NULL)) {
                valid = CDX_FALSE;
                break;
            }

            CDX_LOGV("found subsequent frame #%d at %lld", j + 2, test_pos);

            test_pos += test_frame_size;
        }

        if (valid) {
            *inoutPos = pos;

            if (outHead != NULL) {
                *outHead = header;
            }
        } else {
            CDX_LOGV("no dice, no valid sequence of frames found.");
        }

        ++pos;
        ++tmp;
        --remainingBytes;

    }while (!valid);
    if(valid)
    {
		mp3->mFrameSize  = mFrameSize;
		mp3->mSampleRate = mSampleRate;
		mp3->mChannels   = mNumChannels;
		mp3->mBitRate    = mBitRate * 1E3;
    }
    
    return valid;    
}

static int CdxMp3Init(CdxParserT* Parameter)
{
    MP3ParserImpl *mp3;
    CdxParserT *parser;
	cdx_int32  testlen = 0;
    
    mp3 = (MP3ParserImpl *)Parameter;
    parser = (CdxParserT *)Parameter;
    
    mp3->mFileSize = CdxStreamSize(mp3->stream);
    cdx_bool sucess;
    cdx_int64 Pos = 0;
    cdx_int32 Header = 0;
    cdx_int64 postId3Pos = 0;
	testlen = Mp3PsrGeTID3_V2(parser);
	CdxStreamSeek(mp3->stream,0,SEEK_SET);
	
    sucess = Mp3HeadResync(parser, 0, &Pos, &postId3Pos, &Header);
	CDX_LOGV("Notice!!  testlen:%d,postId3Pos:%lld",testlen,postId3Pos);
    if (!sucess) {
        mp3->mErrno = PSR_OPEN_FAIL;
		pthread_cond_signal(&mp3->cond);
        goto Exit;
    }
    
    mp3->mFirstFramePos  = Pos;
    mp3->mFixedHeader   = Header;
    
    mp3->mXINGSeeker = CreateXINGSeeker(parser, mp3->mFirstFramePos);
    if (mp3->mXINGSeeker == NULL) {
        mp3->mVBRISeeker = CreateVBRISeeker(parser, postId3Pos);
    }
    CDX_LOGV("mXINGSeeker %p, mVBRISeeker %p", mp3->mXINGSeeker, mp3->mVBRISeeker); 
    
    if (mp3->mXINGSeeker != NULL) 
    {    
        mp3->mDuration = mp3->mXINGSeeker->mDurationUs / 1E3;
    } else if (mp3->mVBRISeeker != NULL) {
        mp3->mDuration = mp3->mVBRISeeker->mDurationUs / 1E3;
    } else if (mp3->mFileSize > 0 && mp3->mBitRate != 0) {
        mp3->mDuration = 8000LL * (mp3->mFileSize - mp3->mFirstFramePos) / mp3->mBitRate;
    } else {
        mp3->mDuration = -1;
        CDX_LOGW("Duration can not calculate");
    }
	CdxStreamSeek(mp3->stream,mp3->mFirstFramePos,SEEK_SET);
    mp3->mErrno = PSR_OK;
	pthread_cond_signal(&mp3->cond);
	return 0;
Exit:  
    return -1;

}

static int CdxMp3ParserGetMediaInfo(CdxParserT *parser, CdxMediaInfoT *mediaInfo)
{
    MP3ParserImpl *mp3;
    int ret = 0;    
    AudioStreamInfo *audio = NULL;
    
    mp3 = (MP3ParserImpl *)parser;
    mediaInfo->fileSize     = CdxStreamSize(mp3->stream);
    audio                   = &mediaInfo->program[0].audio[mediaInfo->program[0].audioNum];
    audio->eCodecFormat     = AUDIO_CODEC_FORMAT_MP3;
    audio->nChannelNum      = mp3->mChannels;	    
    audio->nSampleRate      = mp3->mSampleRate;
    audio->nAvgBitrate      = mp3->mBitRate;
    audio->nMaxBitRate      = mp3->mBitRate;
    
    mediaInfo->program[0].audioNum++;
    mediaInfo->program[0].duration  = mp3->mDuration;
    mediaInfo->bSeekable = CdxStreamSeekAble(mp3->stream);
	
	if(mp3->mAlbum_sz < 32 && mp3->mAlbum)
    	memcpy(mediaInfo->album,mp3->mAlbum,mp3->mAlbum_sz);
	if(mp3->mauthor_sz < 32 && mp3->mauthor)
	    memcpy(mediaInfo->author,mp3->mauthor,mp3->mauthor_sz);
	if(mp3->mYear_sz < 32 && mp3->mYear)
		memcpy(mediaInfo->year,mp3->mYear,mp3->mYear_sz);
    if(mp3->mGenre_sz < 32 && mp3->mGenre_sz)
		memcpy(mediaInfo->genre,mp3->mGenre,mp3->mGenre_sz);
    if(mp3->mtitle_sz < 32 && mp3->mtitle)
		memcpy(mediaInfo->title,mp3->mtitle,mp3->mtitle_sz);
    /*I have APIC length and location, do u wanna it????*/
    if(mp3->pAlbumArtBuf != NULL && mp3->mAlbumArtBufSize > 0)
    {
		mediaInfo->pAlbumArtBuf     = mp3->pAlbumArtBuf;
		mediaInfo->nAlbumArtBufSize = mp3->mAlbumArtBufSize;
	}
    return ret;
}

static int CdxMp3ParserControl(CdxParserT *parser, cdx_int32 cmd, void *param)
{
    MP3ParserImpl *mp3;

    mp3 = (MP3ParserImpl *)parser;
    switch (cmd) {
        case CDX_PSR_CMD_DISABLE_AUDIO:
        case CDX_PSR_CMD_DISABLE_VIDEO:
        case CDX_PSR_CMD_SWITCH_AUDIO:
        	break;
        case CDX_PSR_CMD_SET_FORCESTOP:
        {
        	mp3->mErrno = PSR_USER_CANCEL;
        	CdxStreamForceStop(mp3->stream);
          break;
        }
        case CDX_PSR_CMD_CLR_FORCESTOP:
        	CdxStreamClrForceStop(mp3->stream);
        	break;
        default:
            CDX_LOGW("not implement...(%d)", cmd);
            break;
    }

   return 0; 
}

static int CdxMp3ParserPrefetch(CdxParserT *parser, CdxPacketT *pkt)
{
    MP3ParserImpl *mp3;

    mp3 = (MP3ParserImpl *)parser;

    cdx_int64 nReadPos = CdxStreamTell(mp3->stream);
    if (nReadPos >= mp3->mFileSize) {
    	CDX_LOGD("Flie EOS");
        mp3->mErrno = PSR_EOS;
        return -1;
    }

    pkt->type = CDX_MEDIA_AUDIO;
    pkt->length = MP3_PACKET_SIZE;
    pkt->flags |= (FIRST_PART|LAST_PART);
    
    //if (1)//mp3->mSeeking)
	{
        pkt->pts = mp3->mSeekingTime;
        mp3->mSeeking = 0;
    }
    
   	//CDX_LOGV("pkt->pts %lld", pkt->pts);
    return 0;
}

static int CdxMp3ParserRead(CdxParserT *parser, CdxPacketT *pkt)
{
    MP3ParserImpl *mp3;
    int ret = CDX_SUCCESS;
    cdx_int64 nReadPos = 0;
    int nReadSize = MP3_PACKET_SIZE;
    int nRetSize = 0;

    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser prefetch failed!");
        ret = -1;
        goto Exit;
    }

    nReadPos = CdxStreamTell(mp3->stream);
    if (nReadPos < mp3->mFileSize) {
        nReadSize = FFMIN(nReadSize, mp3->mFileSize - nReadPos);
    }
    
    nRetSize= CdxStreamRead(mp3->stream, pkt->buf, nReadSize);
    if(nRetSize < 0)
    {
        CDX_LOGE("File Read Fail");
        mp3->mErrno = PSR_IO_ERR;
        ret = -1;
        goto Exit;
    }
    else if(nRetSize == 0)
    {
       CDX_LOGD("Flie Read EOS");
       mp3->mErrno = PSR_EOS;
       ret = -1;
       goto Exit;
    }    
	pkt->length = nRetSize;
    mp3->readPacketSize += nRetSize;
    
Exit:
    return ret;
}

static int CdxMp3ParserSeekTo(CdxParserT *parser, cdx_int64 timeUs)
{
    MP3ParserImpl *mp3;
    int         ret = 0;
    //cdx_int64   mSeekTime = timeUs;
    cdx_bool    seekCBR = CDX_FALSE;
    cdx_int64   mSamplesRead = 0;
    cdx_int64   mCurrentTimeUs = 0;
    cdx_int64   mBasisTimeUs   = 0;
    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser seekto failed!");
        ret = -1;
        goto Exit;
    }
    
    int64_t actualSeekTimeUs = timeUs;
    if ((mp3->mXINGSeeker == NULL || !GetXINGOffsetForTime(mp3->mXINGSeeker, &actualSeekTimeUs, &mp3->mCurrentPos))
        && (mp3->mVBRISeeker == NULL || !GetVBRIOffsetForTime(mp3->mVBRISeeker, &actualSeekTimeUs, &mp3->mCurrentPos))) {
        if (!mp3->mBitRate) {
            CDX_LOGW("no bitrate");
            ret = -1;
            goto Exit;
        }
        seekCBR = CDX_TRUE;
        mCurrentTimeUs = timeUs;
        mp3->mCurrentPos = mp3->mFirstFramePos + timeUs * mp3->mBitRate / 8000000;
    } else {
        mCurrentTimeUs = actualSeekTimeUs;
    }
    
    mBasisTimeUs = mCurrentTimeUs;  
    CDX_LOGV("mCurrentPos %lld, mBitRate %d, mCurrentTimeUs %lld", mp3->mCurrentPos, mp3->mBitRate, mCurrentTimeUs);

    if (mp3->mCurrentPos > mp3->mFileSize) {
        //return -1;
        mp3->mCurrentPos = mp3->mFileSize;
		CdxStreamSeek(mp3->stream, mp3->mCurrentPos, SEEK_SET);
		mp3->mSeeking   = 1;
    	mp3->mSeekingTime = mCurrentTimeUs;
		goto Exit;
    }

    size_t mFrameSize;
    int mBitRate;
    int mNumSamples;
    int mSampleRate;
    uint8_t buffer[4] = {0};
    for (;;) {
        if (CdxStreamSeek(mp3->stream, mp3->mCurrentPos, SEEK_SET)) {
            return -1;
        }       
        ssize_t n = CdxStreamRead(mp3->stream, buffer, 4);
        if (n < 4) {
			ret = 0;
			goto Exit;
            //return -1;
        }

        uint32_t Header = U32_AT((const uint8_t *)buffer);

        if ((Header & kMask) == (mp3->mFixedHeader & kMask)
            && GetMPEGAudioFrameSize(
                Header, &mFrameSize, &mSampleRate, NULL,
                &mBitRate, &mNumSamples)) {

            // re-calculate mCurrentTimeUs because we might have called Resync()
            if (seekCBR)
            {
                mCurrentTimeUs = (mp3->mCurrentPos - mp3->mFirstFramePos) * 8000 * 1000 / mp3->mBitRate;
                mBasisTimeUs = mCurrentTimeUs;        
                CDX_LOGV("seekCBR mCurrentTimeUs %lld", mCurrentTimeUs);
            }

            break;
        }

        // Lost sync.
        CDX_LOGV("lost sync! header = 0x%08x, old header = 0x%08x\n", Header, mp3->mFixedHeader);

        cdx_int64 Pos = mp3->mCurrentPos;
        if (!Mp3HeadResync(parser, mp3->mFixedHeader, &Pos, NULL, NULL)) {
            CDX_LOGE("Unable to resync. Signalling end of stream.");
			ret = 0;
            goto Exit;
        }

        mp3->mCurrentPos = Pos;
    }
    
    if (CdxStreamSeek(mp3->stream, mp3->mCurrentPos, SEEK_SET)) {
        ret = -1;
        goto Exit;
    }          
    
    mSamplesRead += mNumSamples;
    mCurrentTimeUs = mBasisTimeUs + ((mSamplesRead * 1000000) / mp3->mSampleRate);
    
    mp3->mSeeking   = 1;
    mp3->mSeekingTime = mCurrentTimeUs;
    mp3->mCurrentPos += mFrameSize;
    CDX_LOGV("mSeekingTime %lld", mp3->mSeekingTime);   
Exit:    
    return ret;
}

static cdx_uint32 CdxMp3ParserAttribute(CdxParserT *parser)
{
    MP3ParserImpl *mp3;
    int ret = 0;

    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser Attribute failed!");
        ret = -1;
        goto Exit;
    }
    
Exit:    
    return ret;
}

#if 0
static int CdxMp3ParserForceStop()
{
    return 0;
}
#endif
static int CdxMp3ParserGetStatus(CdxParserT *parser)
{
    MP3ParserImpl *mp3;

    mp3 = (MP3ParserImpl *)parser;
#if 0
    if (CdxStreamEos(mp3->stream)) {
        CDX_LOGE("File EOS! ");
        return mp3->mErrno = PSR_EOS;
    }
#endif	
    return mp3->mErrno;
}

static int CdxMp3ParserClose(CdxParserT *parser)
{
    MP3ParserImpl *mp3;
    mp3 = (MP3ParserImpl *)parser;
    if (!mp3) {
        CDX_LOGE("Mp3 file parser prefetch failed!");
        return -1;
    }
	if(mp3->mInforBuftemp)
	{
	   free(mp3->mInforBuftemp);	
	}
/*    if (mp3->mVBRISeeker && mp3->mVBRISeeker->mSegments) {*/
/*        free(mp3->mVBRISeeker->mSegments);*/
/*        mp3->mVBRISeeker->mSegments = NULL;*/
/*    }*/
    if (mp3->mVBRISeeker) {
        free(mp3->mVBRISeeker);
        mp3->mVBRISeeker = NULL;
    }

    if (mp3->mXINGSeeker) {
        free(mp3->mXINGSeeker);
        mp3->mXINGSeeker = NULL;
    }

	if(mp3->pAlbumArtBuf)
	{
		free(mp3->pAlbumArtBuf);
		mp3->pAlbumArtBuf = NULL;
	}
    
#if ENABLE_FILE_DEBUG
    if (mp3->teeFd) {
        close(mp3->teeFd);
    }
#endif
	if(mp3->stream) {
		CdxStreamClose(mp3->stream);
	}
	pthread_cond_destroy(&mp3->cond);
    CdxFree(mp3);
    return 0;
}

static struct CdxParserOpsS Mp3ParserImpl = 
{
    .control      = CdxMp3ParserControl,
    .prefetch     = CdxMp3ParserPrefetch,
    .read         = CdxMp3ParserRead,
    .getMediaInfo = CdxMp3ParserGetMediaInfo,
    .seekTo       = CdxMp3ParserSeekTo,
    .attribute    = CdxMp3ParserAttribute,
    .getStatus    = CdxMp3ParserGetStatus,
    .close        = CdxMp3ParserClose,
    .init         = CdxMp3Init
};

CdxParserT *CdxMp3ParserOpen(CdxStreamT *stream, cdx_uint32 flags)
{
    MP3ParserImpl *Mp3ParserImple;
    //int ret = 0;
    if(flags > 0) {
        CDX_LOGI("Flag Not Zero");
    }
    Mp3ParserImple = (MP3ParserImpl *)malloc(sizeof(MP3ParserImpl));
    if (Mp3ParserImple == NULL) {
        CDX_LOGE("Mp3ParserOpen Failed");
		CdxStreamClose(stream);
        return NULL;
    }
    memset(Mp3ParserImple, 0, sizeof(MP3ParserImpl));
    
    Mp3ParserImple->stream = stream;
    Mp3ParserImple->base.ops = &Mp3ParserImpl;
    Mp3ParserImple->mErrno = PSR_INVALID;
	pthread_cond_init(&Mp3ParserImple->cond, NULL);
    //ret = pthread_create(&Mp3ParserImple->openTid, NULL, Mp3OpenThread, (void*)Mp3ParserImple);
    //CDX_FORCE_CHECK(!ret);
    
#if ENABLE_FILE_DEBUG
    char teePath[64];
    strcpy(teePath, "/data/camera/mp3.dat");
    Mp3ParserImple->teeFd = open(teePath, O_WRONLY | O_CREAT | O_EXCL, 0775);   
#endif

    return &Mp3ParserImple->base;
}

static cdx_uint32 Mp3Probe(CdxStreamProbeDataT *p) 
{
    return Resync(p)*100;
    /*
    int ret = 0;

    if (((ret = FFMp3Probe(p)) >= AVPROBE_SCORE_MAX / 4 - 1)|| Resync(p))
        return CDX_TRUE; 

	return CDX_FALSE;
	*/
}

static cdx_uint32 CdxMp3ParserProbe(CdxStreamProbeDataT *probeData)
{
    if (probeData->len < 2/*MP3_PROBE_SIZE*/) {
        CDX_LOGE("Mp3 Probe Failed");
        return 0;
    }

    return Mp3Probe(probeData);
}

CdxParserCreatorT mp3ParserCtor = 
{
    .create = CdxMp3ParserOpen,
    .probe = CdxMp3ParserProbe
};

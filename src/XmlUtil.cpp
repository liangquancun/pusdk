#include "XmlUtil.h"
#include "md5.h"
#include "TStringUtil.h"
#include "log.h"
#include "ms_pu_param.h"

std::string structToXml(const vag::AlarmNotification* alarm)
{
	TiXmlDocument *myDocument = new TiXmlDocument();
	TiXmlDeclaration Declaration( "1.0","UTF-8", "yes" );
	myDocument->InsertEndChild( Declaration );
	TiXmlElement *RootElement = NULL;	
	RootElement = new TiXmlElement("request");
	myDocument->LinkEndChild(RootElement);
	RootElement->SetAttribute("command", "RaiseAlarm");

	TiXmlElement* parameters = 
		RootElement->InsertEndChild( TiXmlElement( "parameters") )->ToElement();	

	TiXmlElement *puId = 
		parameters->InsertEndChild(TiXmlElement("puId"))->ToElement();
	puId->InsertEndChild( TiXmlText(alarm->pu_id));

	TiXmlElement *puIp = 
		parameters->InsertEndChild(TiXmlElement("puIp"))->ToElement();
	puIp->InsertEndChild( TiXmlText(alarm->pu_ip));


	char type[32] = {0};
	if(alarm->alarm_type == CAM_MOTIONDETECT )
		strncpy(type, "CAM_MOTIONDETECT", 32);
	else if(alarm->alarm_type == CAM_VIDEOLOSE )
		strncpy(type, "CAM_VIDEOLOSE", 32);
	else if(alarm->alarm_type == VS_GPIN)
		strncpy(type, "VS_GPIN", 32);
	else if(alarm->alarm_type == CAM_HIDEDETECT)
		strncpy(type, "CAM_HIDEDETECT", 32);
	else if(alarm->alarm_type == VS_DISKERROR)
		strncpy(type, "VS_DISKERROR", 32);
	else if(alarm->alarm_type == VS_DISKFULL)
		strncpy(type, "VS_DISKFULL", 32);
	else if(alarm->alarm_type == CAM_PIRMOTION)
		strncpy(type, "CAM_PIRMOTION", 32);
	else if(alarm->alarm_type == VCA_INTRUSION)
		strncpy(type, "VCA_INTRUSION", 32);

	TiXmlElement *alarmType = 
		parameters->InsertEndChild(TiXmlElement("alarmType"))->ToElement();
	alarmType->InsertEndChild( TiXmlText(type));

	TiXmlElement *serverType = 
		parameters->InsertEndChild(TiXmlElement("serverType"))->ToElement();
	serverType->InsertEndChild( TiXmlText(alarm->server_type));

	char disk_number[32] = {0};
	sprintf(disk_number, "%d", alarm->disk_number);
	TiXmlElement *diskNumber = 
		parameters->InsertEndChild(TiXmlElement("diskNumber"))->ToElement();
	diskNumber->InsertEndChild( TiXmlText(disk_number));

	TiXmlElement *timeStamp = 
		parameters->InsertEndChild(TiXmlElement("timeStamp"))->ToElement();
	timeStamp->InsertEndChild( TiXmlText(alarm->time_stamp));

	char void_count[32] = {0};
	sprintf(void_count, "%d", alarm->data.integerValue);
	TiXmlElement *data = 
		parameters->InsertEndChild(TiXmlElement("data"))->ToElement();
	data->InsertEndChild( TiXmlText(void_count));

	TiXmlElement *eliminated = 
		parameters->InsertEndChild(TiXmlElement("eliminated"))->ToElement();
	eliminated->InsertEndChild( TiXmlText(alarm->eliminated));
	//ss<<*myDocument;
    TiXmlPrinter printer; 
    myDocument->Accept(&printer); 
    std::string ss(printer.CStr());
    delete myDocument;
    printf("ss:%s\n",ss.c_str());
	return ss;
}

std::string structToXml(const vag::VAGRegInfo &regInfo)
{ 
	TiXmlDocument *myDocument = new TiXmlDocument();
	TiXmlDeclaration *Declaration = new TiXmlDeclaration( "1.0","UTF-8", "yes" );
    myDocument->InsertEndChild( *Declaration );
    TiXmlElement *RootElement = new TiXmlElement("request");	
    myDocument->LinkEndChild(RootElement);
  	TiXmlElement* parameters = 
		RootElement->InsertEndChild( TiXmlElement( "parameters") )->ToElement();	
	RootElement->SetAttribute("command", "PuRegister");

	TiXmlElement *puId = 
		parameters->InsertEndChild(TiXmlElement("puId"))->ToElement();
    puId->InsertEndChild( TiXmlText(regInfo.VagId));


	TiXmlElement *puIp = 
		parameters->InsertEndChild(TiXmlElement("puIp"))->ToElement();
    puIp->InsertEndChild( TiXmlText(regInfo.VagIp));


	TiXmlElement *puSigPort = 
		parameters->InsertEndChild(TiXmlElement("puSigPort"))->ToElement();
    puSigPort->InsertEndChild( TiXmlText("5060"));

	TiXmlElement *puName = 
		parameters->InsertEndChild(TiXmlElement("puName"))->ToElement();
    puName->InsertEndChild( TiXmlText(regInfo.VagUserName));

    //comn::CMD5 md5;
    MD5 md5;
	md5.update((char*)regInfo.VagPassword, strlen(regInfo.VagPassword));
    TiXmlElement *puPassword = 
		parameters->InsertEndChild(TiXmlElement("puPassword"))->ToElement();
    puPassword->InsertEndChild( TiXmlText(
		md5.toString().c_str()));
	//puPassword->InsertEndChild(TiXmlText("123456"));

	TiXmlElement *puType = 
		parameters->InsertEndChild(TiXmlElement("puType"))->ToElement();
	puType->InsertEndChild( TiXmlText("1"));
   
	TiXmlElement *manufacturerId = 
		parameters->InsertEndChild(TiXmlElement("manufacturerId"))->ToElement();
    manufacturerId->InsertEndChild( TiXmlText("mr"));

    TiXmlPrinter printer; 
    myDocument->Accept(&printer);
    std::string ss(printer.CStr());
    delete myDocument;  
	return ss;
}

std::string structToXml(const CmuKeepAlive &alive)
{
	TiXmlDocument *myDocument = new TiXmlDocument();
	TiXmlDeclaration Declaration( "1.0","UTF-8", "yes" );
	myDocument->InsertEndChild( Declaration );
	TiXmlElement *RootElement = NULL;	
	RootElement = new TiXmlElement("request");
	myDocument->LinkEndChild(RootElement);
	RootElement->SetAttribute("command", "PuKeepAlive");

	TiXmlElement* parameters = 
		RootElement->InsertEndChild( TiXmlElement( "parameters") )->ToElement();	

	TiXmlElement *puId = 
		parameters->InsertEndChild(TiXmlElement("puId"))->ToElement();
	puId->InsertEndChild( TiXmlText(alive.m_puId.c_str()));

   
	 TiXmlPrinter printer; 
    myDocument->Accept(&printer); 
     std::string ss(printer.CStr());
     
    //std::string ss;
	//ss<<*myDocument;
	char strTime[128];
    memset(strTime,0,sizeof(strTime));
	getLocalTime(strTime);
    //printf("time:%s \nkeepalive:%s\n",strTime,ss.c_str());
    delete myDocument;
    
	return ss;
}


bool structFromXmlElement(MvRegInfoEx& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{		
		if(   (child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")) )
		{
			for(TiXmlNode* child2 = child->FirstChild(); child2; 
				child2 = child2->NextSibling())
			{				
				if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "sipRegIPAddr")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_mduIp.assign(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "puKeepAlivePeriod")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_keepAlivePeriod = atoi(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "sipRegPort")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_mduPort = atoi(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "puKeepAlivePort")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_puKeepAlivePort = atoi(node->GetText());
					}					
				}
			}
		}
		else if( (child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"result")) )
		{
			node = (TiXmlElement*)child;
			node->Attribute("code",&obj.m_code)	;
		}
	}
	return obj.m_code == 0;
}

bool structFromXmlElement(ControlPtzMsg& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{		
		if(   (child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")) )
		{
			for(TiXmlNode* child2 = child->FirstChild(); child2; 
				child2 = child2->NextSibling())
			{				
				if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "puId")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_puId.assign(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "ptzId")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_videoId = atoi(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "cmd")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_cmd.assign(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "param")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_data.assign(node->GetText());
					}					
				}
				else if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(), "speed")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText() != NULL)
					{
						obj.m_speed = atoi(node->GetText());
					}					
				}
			}
		}
		
	}
	return !obj.m_cmd.empty();
}

std::string structToXml(unsigned maxPage,
						unsigned curPage,
						const std::string &vagid,
						const VagResNodeArray &resLst)
{
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","GB2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = NULL;
	resRootElement = new TiXmlElement("request");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", "DeviceReport");

	TiXmlElement* resparameters = 
		resRootElement->InsertEndChild( TiXmlElement("parameters") )->ToElement();

	TiXmlElement *vagDeviceId = 
		resparameters->InsertEndChild(TiXmlElement("vagDeviceId"))->ToElement();
	vagDeviceId->InsertEndChild(TiXmlText(vagid.c_str()));

    std::string aa(comn::StringUtil::format("%u",maxPage));
	TiXmlElement *resDeviceNum = 
		resparameters->InsertEndChild(TiXmlElement("totalNum"))->ToElement();
	resDeviceNum->InsertEndChild(TiXmlText(aa.c_str()));

     aa = comn::StringUtil::format("%u",curPage);
	TiXmlElement *indexNum = 
		resparameters->InsertEndChild(TiXmlElement("indexNum"))->ToElement();
	indexNum->InsertEndChild(TiXmlText(aa.c_str()));

     aa = comn::StringUtil::format("%u",resLst.size());
	TiXmlElement *deviceNum = 
		resparameters->InsertEndChild(TiXmlElement("deviceNum"))->ToElement();
	deviceNum->InsertEndChild(TiXmlText(aa.c_str()));
	
	VagResNodeArray::const_iterator it = resLst.begin();
	for (; it != resLst.end(); it++)
	{		
		/*TiXmlElement* camera = 
			resparameters->InsertEndChild( TiXmlElement( "camera") )->ToElement();
		camera->SetAttribute("ptzEnable","1");
		camera->SetAttribute("installLocation","0,0");
		camera->SetAttribute("platformFlag", vagid);
		camera->SetAttribute("deviceStatus",comn::StringUtil::format("%d",it->status));	//设备默认不在线状态 1
		camera->SetAttribute("channelsId",comn::StringUtil::format("%d",it->videoId));
		camera->SetAttribute("puName",it->name);
		camera->SetAttribute("puId",it->puid);
		camera->SetAttribute("deviceType","0");
		camera->SetAttribute("deviceName",it->name);
		camera->SetAttribute("vagChId",comn::StringUtil::format("%d",it->videoId));
		camera->SetAttribute("deviceId",it->puid);
		*/
	}


     TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
     std::string ss(printer.CStr());
    delete resmyDocument;
    
	return ss;
}

bool structFromXmlElement(StartMediaReq& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{		
		if(    (child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")) )
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; 
				child2=child2->NextSibling())
			{
				if(   (child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
				   && (!strcmp(child2->Value(),"puId")) )
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.m_puId.assign(node->GetText());
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
                        
						obj.m_videoId = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"mediaType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_iMediaType = atoi(node->GetText());
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"streamType")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{

						obj.m_iStreamType = atoi(node->GetText());

					}										
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"linkMode")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_iLinkMode = atoi(node->GetText());
					}										
				}
                else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"playType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_playType= atoi(node->GetText());
					}										
				}   
                else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"fileName")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_fileName.assign(node->GetText());
					}										
				}
                else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"startTime")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_startTime.assign(node->GetText());
					}										
				} 
                else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"stopTime")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_endTime.assign(node->GetText());
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"targetIPAddr")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_szTargetIp.assign(node->GetText());
					}										
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"targetPort")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_targetPort = atoi(node->GetText());
					}										
				}
			}//for
		}			
	}//for

	return !obj.m_puId.empty();
}


bool structFromXmlElement(StopMediaReq& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"puId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.m_puId = node->GetText();
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.m_videoId = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"streamType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_iStreamType = atoi(node->GetText());
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"fileName")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_fileName = node->GetText();
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"mediaType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_iMediaType = atoi(node->GetText());
					}										
				}
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(DeletePtzPreset& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"ptzId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.ptz_id =  atoi(node->GetText());
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"presetIndex")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.preset_index = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"presetName")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.preset_name, node->GetText(), sizeof(obj.preset_name));
					}										
				}
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(PtzPresets& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"ptzId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.ptz_id =  atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"presetIndex")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.preset_lists->preset_index = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"presetName")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.preset_lists->preset_name, node->GetText(), sizeof(obj.preset_lists->preset_name));
					}										
				}
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(CMU_DEV_TIME& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hour")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.hour = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"minute")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.minute = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"second")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.second = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"date")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.date = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"month")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.month = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"year")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.year = atoi(node->GetText());
					}										
				}
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(CMU_IP_INFO& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"netId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.net_id = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"ipAddress")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						strncpy(obj.ip_addr, node->GetText(), sizeof(obj.ip_addr));
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"controlPort")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.ctrl_port = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"subnetMask")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.net_mask, node->GetText(), sizeof(obj.net_mask));
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"dns")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.dns_addr[0], node->GetText(), sizeof(obj.dns_addr[0]));
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"gateway")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.gate_way, node->GetText(), sizeof(obj.gate_way));
					}										
				}
			}//for
		}		
	}//for

	return true;
}


bool structFromXmlElement(CMU_HIDE_DETECT& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionAlarmEnabled")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						strncpy(obj.alarm_enabled, node->GetText(), sizeof(obj.alarm_enabled));
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionAlarmTime")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.alarm_time, node->GetText(), sizeof(obj.alarm_time));
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionX")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.hide_coord.x = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionY")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.hide_coord.y = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionWidth")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.hide_coord.width = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionHeight")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.hide_coord.height = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionSensitivity")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.sensitivity = atoi(node->GetText());
					}										
				}

				
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionAlarmOutput")))
				{
					//
					node = (TiXmlElement*)child2;
					for(TiXmlNode* child3 = node->FirstChild(); child3; child3=child3->NextSibling())
					{
						if((child3->Type() == TiXmlNode::TINYXML_ELEMENT) 
							&& (!strcmp(child3->Value(),"alarmOutput")))
						{
							node = (TiXmlElement*)child3;
							if (node->GetText())
							{
								obj.hide_alarm_output.alarm_channel = atoi(node->GetText());
							}
						}

						if((child3->Type() == TiXmlNode::TINYXML_ELEMENT) 
							&& (!strcmp(child3->Value(),"alarmState")))
						{
							node = (TiXmlElement*)child3;
							if (node->GetText())
							{
								obj.hide_alarm_output.alarm_time = atoi(node->GetText());
							}
						}
					}//for
					//
				}


				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hideDetectionVedio")))
				{
					//
					node = (TiXmlElement*)child2;
					for(TiXmlNode* child3 = node->FirstChild(); child3; child3=child3->NextSibling())
					{
						if((child3->Type() == TiXmlNode::TINYXML_ELEMENT) 
							&& (!strcmp(child3->Value(),"hideDetection")))
						{
							node = (TiXmlElement*)child3;
							if (node->GetText())
							{
								obj.hide_alarm_video.alarm_record_channel = atoi(node->GetText());
							}
						}

						if((child3->Type() == TiXmlNode::TINYXML_ELEMENT) 
							&& (!strcmp(child3->Value(),"AlarmRecord")))
						{
							node = (TiXmlElement*)child3;
							if (node->GetText())
							{
								obj.hide_alarm_video.alarm_shot_channel = atoi(node->GetText());
							}
						}
					}//for
					//										
				}
			}//for
		}		
	}//for

	return true;
}
bool structFromXmlElement(CMU_VIDEO_FORMAT& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"encodeMode")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_format = atoi(node->GetText());
					}
				}
			}//for
		}		
	}//for

	return true;
}
bool structFromXmlElement(CMU_IMG_DISPLAY& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"contrast")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.contrast = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bright")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.bright = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"hue")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.hue = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"saturation")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.saturation = atoi(node->GetText());
					}										
				}
			}//for
		}		
	}//for

	return true;
}
bool structFromXmlElement(CMU_SERIAL_PORT& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"forDecoder")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						strncpy(obj.for_decoder, node->GetText(), sizeof(obj.for_decoder));
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"serialPort")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.serial_port = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"workMode")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.work_mode = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"decoderType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.decoder_type = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"decoderAddress")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.decoder_address = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"baudRate")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.baud_rate = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"dataBit")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.data_bit = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"parity")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.parity = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"stopBit")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.stop_bit = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"timeOut")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.time_out = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"flowControl")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.flow_control = atoi(node->GetText());
					}										
				}
			}//for
		}		
	}//for

	return true;
}
bool structFromXmlElement(CMU_IMG_ENCODE& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"encodeMode")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						strncpy(obj.encode_mode, node->GetText(), sizeof(obj.encode_mode));
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"picQuality")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.pic_quality = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitRate")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.bit_rate = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitRateType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.bit_rate_type = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"frameRate")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.frame_rate = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"imageSize")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.image_size, node->GetText(), sizeof(obj.image_size));
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"streamType")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.stream_type = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"iFrameInterval")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.iframe_interval = atoi(node->GetText());
					}										
				}
			}//for
		}		
	}//for

	return true;
}
bool structFromXmlElement(CMU_IMG_TEXT& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}
				}	

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapTimeEnable")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						strncpy(obj.time_enable, node->GetText(), sizeof(obj.time_enable));
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapX")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.time_X = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapY")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.time_Y = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapTextEnable")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.text_enable, node->GetText(), sizeof(obj.text_enable));
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapText")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						strncpy(obj.text, node->GetText(), sizeof(obj.text));
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapTextX")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.text_X = atoi(node->GetText());
					}										
				}

				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"bitmapTextY")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.text_Y = atoi(node->GetText());
					}										
				}
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(CMU_OPER_CTRL_DEV& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"controlAction")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.control_action = atoi(node->GetText());
					}
				}	
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(CMU_OPER_CTRL_ALARM& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"puId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
                        memcpy(obj.pu_id,node->GetText(),sizeof(obj.pu_id));
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"armingType")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.alarm_type= atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"enableArming")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
                        memcpy(obj.enable_arming,node->GetText(),sizeof(obj.enable_arming));
                    }
				}	
			}//for
		}		
	}//for

	return true;
}

bool structFromXmlElement(CMU_QUERY_STORE_FILE& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"puId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.video_id= atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"cuId")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"fromDate")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
                       sscanf(node->GetText(), "%04d-%02d-%02d %02d:%02d:%02d",
                           &(obj.from_date.year),
                           &(obj.from_date.month),
                           &(obj.from_date.date),
                           &(obj.from_date.hour),
                           &(obj.from_date.minute),
                           &(obj.from_date.second)
                           );
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"toDate")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
                       sscanf(node->GetText(), "%04d-%02d-%02d %02d:%02d:%02d",
                           &(obj.to_date.year),
                           &(obj.to_date.month),
                           &(obj.to_date.date),
                           &(obj.to_date.hour),
                           &(obj.to_date.minute),
                           &(obj.to_date.second)
                           );
					}										
				}
			}//for
		}		
	}//for

	return true;
}



bool structFromXmlElement(PlayBackReq& obj, TiXmlElement* node)
{
	for(TiXmlNode* child = node->FirstChild(); child; child=child->NextSibling())
	{			
		if((child->Type() == TiXmlNode::TINYXML_ELEMENT) 
			&& (!strcmp(child->Value(),"parameters")))
		{
			node = (TiXmlElement*)child;
			for(TiXmlNode* child2 = node->FirstChild(); child2; child2=child2->NextSibling())
			{
				if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"puId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.m_puId.assign(node->GetText());
					}
				}	
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"videoId")))
				{
					node = (TiXmlElement*)child2;
					if (node->GetText())
					{
						obj.m_videoId = atoi(node->GetText());
					}
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"cuId")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_cuId.assign(node->GetText());
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"fileName")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_fileName.assign(node->GetText());
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"cmd")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_cmd.assign(node->GetText());
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"param")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_param= atoi(node->GetText());
					}										
				}
	           
                else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"sessionId")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_cuSessId.assign(node->GetText());
					}										
				}
                else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"dragValue")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_dragValue = atoi(node->GetText());
					}										
				}
				else if((child2->Type() == TiXmlNode::TINYXML_ELEMENT) 
					&& (!strcmp(child2->Value(),"playRate")))
				{
					node = (TiXmlElement*)child2;					
					if (node->GetText())
					{
						obj.m_playRate = atoi(node->GetText());
					}										
				}
            }//for
		}		
	}//for

	return true;
}





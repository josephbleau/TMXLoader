#include "TMXLoader.h"
#include "TMXMap.h"

#include <sstream>

TMXLoader::TMXLoader(std::string filename, TMXMap *map)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	this->parsed = false;
	this->last_layer = NULL;
	this->last_tileset = NULL;
	this->last_object_group = NULL;
	this->map = map;

	doc = xmlParseFile(filename.c_str());
	if( doc == NULL ) {
		std::cerr << "Unable to parse file " << filename << std::endl;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		std::cerr << "Unable to parse file " << filename 
			      << ", it's empty." << std::endl;

		xmlFreeDoc(doc);
	}
	else{
		if(xmlStrcmp(cur->name, (const xmlChar *) "map")){
			std::cerr << "Malformed XML, root node should be 'map'" 
				      << std::endl;

			xmlFreeDoc(doc);
		}
		else{
			this->node_ptr = cur;
			this->doc_ptr = doc;
		}
	}
}

TMXLoader::~TMXLoader()
{
}

void TMXLoader::Parse()
{
	if(this->parsed)
		return;
	if(this->doc_ptr == NULL || 
	   this->node_ptr == NULL )
	   return;

	bool in_map, in_tileset, in_image, in_layer, in_data;
	in_map = in_tileset = in_image = in_layer = in_data = false;
		
	xmlNodePtr p = this->node_ptr;
	while(p != NULL)
	{
		if(_DEBUG_)
			std::cout << "entering " << p->name << std::endl;
		
		if( xmlStrcmp(p->name, (xmlChar*) "map") == 0 ){
			this->ParseMapTag(p);
		}
		else if( xmlStrcmp(p->name, (xmlChar*) "tileset") == 0 ){
			this->ParseTilesetTag(p);
		}
		else if( xmlStrcmp(p->name, (xmlChar*) "image") == 0){
			this->ParseImageTag(p);
		}
		else if( xmlStrcmp(p->name, (xmlChar*) "layer") == 0){
			this->ParseLayerTag(p);
		}
		else if( xmlStrcmp(p->name, (xmlChar*) "data") == 0){
			if(p->children)
				p = p->children;

			if( xmlStrcmp(p->name, (xmlChar*) "text") == 0){
				xmlChar *text;
				
				text = xmlNodeListGetString(this->doc_ptr, p, 1);		
				std::string data = (char*) text;

				this->last_layer->data = this->ParseDataText(data);
			}

		}
		else if( xmlStrcmp(p->name, (xmlChar*) "objectgroup") == 0){
			this->ParseObjectGroupTag(p);
		}
		else if( xmlStrcmp(p->name, (xmlChar*) "object") == 0){
			this->ParseObjectTag(p);
		}
		else if( xmlStrcmp(p->name, (xmlChar*) "property") == 0){
			this->ParsePropertyTag(p);
		}
	
		/* Hippity hoppity all over this tree. */
		if(p->children != NULL)
			p = p->children;
		else if(p->next != NULL)
			p = p->next;
		else if(p->parent->next != NULL)
			p = p->parent->next;
		else
			p = NULL;
	}
}

void TMXLoader::ParseMapTag(xmlNodePtr p)
{
	std::stringstream ss;
	xmlChar *attr;
			
	std::string version;
	TMXOrientation orientation;
	int width, height, tilewidth, tileheight;

	attr = xmlGetProp(p, (const xmlChar*) "version");
	version = (const char*) attr;

	attr = xmlGetProp(p, (const xmlChar*) "orientation");
	if( xmlStrcmp(attr, (const xmlChar*) "orthogonal") == 0)
		orientation = ORTHOGONAL;
	else if (xmlStrcmp(attr, (const xmlChar*) "isometric") == 0)
		orientation = ISOMETRIC;

	attr = xmlGetProp(p, (const xmlChar*) "width");
	ss << (const char*) attr;
	ss >> width;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "height");
	ss << (const char*) attr;
	ss >> height;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "tilewidth");
	ss << (const char*) attr;
	ss >> tilewidth;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "tileheight");
	ss << (const char*) attr;
	ss >> tileheight;
	ss.clear();
	
	this->map->version = version;
	this->map->orientation = orientation;
	this->map->height = height;
	this->map->width = width;
	this->map->tileheight = tileheight;
	this->map->tilewidth = tilewidth;
}

void TMXLoader::ParseTilesetTag(xmlNodePtr p)
{
	std::stringstream ss; /* for conversions */
	xmlChar *attr;

	int firstgid, tilewidth, tileheight;
	std::string name;

	attr = xmlGetProp(p, (xmlChar*) "name");
	name = (const char*) attr;
			
	attr = xmlGetProp(p, (xmlChar*) "firstgid");
	ss << (const char*) attr;
	ss >> firstgid;
	ss.clear();

	attr = xmlGetProp(p, (xmlChar*) "tilewidth");
	ss << ((const char*) attr);
	ss >> tilewidth;
	ss.clear();

	attr = xmlGetProp(p, (xmlChar*) "tileheight");
	ss << (const char*) attr;
	ss >> tileheight;
	ss.clear();

	TMXTileset *t = new TMXTileset(firstgid, name, 
				                    tilewidth, tileheight);
	this->last_tileset = t;
	this->map->tilesets.push_back(t);
}

void TMXLoader::ParseImageTag(xmlNodePtr p)
{
	std::stringstream ss;
	xmlChar *attr;

	std::string source;
	int width, height;

	attr = xmlGetProp(p, (const xmlChar*) "source");
	source = (const char*) attr;

	attr = xmlGetProp(p, (const xmlChar*) "width");
	ss << (const char*) attr;
	ss >> width;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "height");
	ss << (const char*) attr;
	ss >> height;
	ss.clear();

	TMXImage *t = new TMXImage(source, width, height);
	if( this->last_tileset != NULL )
	{
		this->last_tileset->image = t;
	}
}

void TMXLoader::ParseLayerTag(xmlNodePtr p)
{
	std::stringstream ss;
	xmlChar *attr;

	std::string name;
	int width, height;

	attr = xmlGetProp(p, (const xmlChar*) "name");
	name = (const char*) attr;
			
	attr = xmlGetProp(p, (const xmlChar*) "width");
	ss << (const char*) attr;
	ss >> width;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "height");
	ss << (const char*) attr;
	ss >> height;
	ss.clear();

	TMXLayer *l = new TMXLayer(name, width, height);
	this->last_layer = l;
	this->map->layers.push_back(l);
}

void TMXLoader::ParseObjectGroupTag(xmlNodePtr p)
{
	std::stringstream ss;
	xmlChar *attr;

	std::string name;
	int width, height;

	attr = xmlGetProp(p, (const xmlChar*) "name");
	name = (const char*) attr;

	attr = xmlGetProp(p, (const xmlChar*) "width");
	ss << (const char*) attr;
	ss >> width;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "height");
	ss << (const char*) attr;
	ss >> height;
	ss.clear();

	this->last_object_group = new TMXObjectGroup(name, width, height);
	this->map->objectgroups.push_back(this->last_object_group);
}

void TMXLoader::ParseObjectTag(xmlNodePtr p)
{
	std::stringstream ss;
	xmlChar *attr;

	std::string name;
	std::string type;
	int x, y, width = 0, height = 0, gid = -1;

	attr = xmlGetProp(p, (const xmlChar*) "name");
	if(attr)
		name = (const char*) attr;
	
	attr = xmlGetProp(p, (const xmlChar*) "type");
	if(attr)
		type = (const char*) attr;

	attr = xmlGetProp(p, (const xmlChar*) "x");
	ss << (const char*) attr;
	ss >> x;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "y");
	ss << (const char*) attr;
	ss >> y;
	ss.clear();

	attr = xmlGetProp(p, (const xmlChar*) "width");
	if(attr){
		ss << (const char*) attr;
		ss >> width;
		ss.clear();
	} else {
		width = this->map->tilewidth;
	}

	attr = xmlGetProp(p, (const xmlChar*) "height");
	if(attr){
		ss << (const char*) attr;
		ss >> height;
		ss.clear();
	} else {
		width = this->map->tileheight;
	}

	attr = xmlGetProp(p, (const xmlChar*) "gid");
	if(attr){
		ss << (const char*) attr;
		ss >> gid;
		ss.clear();
	} else {
		gid = -1;
	}

	TMXObject *object = new TMXObject(name, type, x, y-this->map->tileheight, width, height, gid);
	this->last_object = object;
	this->last_object_group->objects.push_back(object);
}

void TMXLoader::ParsePropertyTag(xmlNodePtr p)
{
	std::string name;
	std::string value;
	xmlChar *attr;

	attr = xmlGetProp(p, (const xmlChar*) "name");
	name = (const char*) attr;

	attr = xmlGetProp(p, (const xmlChar*) "value");
	value = (const char*) attr;

	this->last_object->properties[name] = value;
}

std::vector<int> TMXLoader::ParseDataText(std::string data)
{
	std::vector<int> ret;
	std::string buf;
	std::stringstream ss;

	for(unsigned i = 0; i < data.length(); i++)
	{
		if(data[i] >= '0' && data[i] <= '9'){
			buf += data[i];
		} else {
			int val = -1;
		
			ss = std::stringstream(buf);
			ss >> val;
			
			buf = "";
			
			if(val != -1) /* I don't fucking know. */
				ret.push_back(val);
		}
	}

	return ret;
}


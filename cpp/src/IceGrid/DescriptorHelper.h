// **********************************************************************
//
// Copyright (c) 2003-2006 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_GRID_DESCRIPTOR_HELPER_H
#define ICE_GRID_DESCRIPTOR_HELPER_H

#include <IceUtil/OutputUtil.h>
#include <IceXML/Parser.h>
#include <IceGrid/Admin.h>

namespace IceGrid
{

class ApplicationHelper;
class Resolver
{
public:

    Resolver(const ApplicationHelper&);
    Resolver(const Resolver&, const std::map<std::string, std::string>&, bool, 
	     const PropertySetDescriptorDict& = PropertySetDescriptorDict());
    Resolver(const std::string&, const std::map<std::string, std::string>&);

    std::string operator()(const std::string&, const std::string& = std::string(), bool = true, bool = true) const;
    std::string asInt(const std::string&, const std::string& = std::string()) const;
    std::string asFloat(const std::string&, const std::string& = std::string()) const;
    void setReserved(const std::string&, const std::string&);
    void setContext(const std::string&);
    const PropertySetDescriptor& getPropertySet(const std::string&) const;
    PropertyDescriptorSeq getProperties(const Ice::StringSeq&) const;

    void addIgnored(const std::string&);

    void exception(const std::string&) const;

    TemplateDescriptor getServerTemplate(const std::string&) const;
    TemplateDescriptor getServiceTemplate(const std::string&) const;
    bool hasReplicaGroup(const std::string&) const;

private:

    std::string substitute(const std::string&, bool = false) const;
    std::string getVariable(const std::string&, bool, bool&) const;
    PropertyDescriptorSeq getProperties(const Ice::StringSeq&, std::set<std::string>&) const;

    static std::map<std::string, std::string> getReserved();
    void checkReserved(const std::string&, const std::map<std::string, std::string>&) const;

    const ApplicationHelper* _application;
    const bool _escape;
    std::string _context;
    std::map<std::string, std::string> _variables;
    std::map<std::string, std::string> _parameters;
    PropertySetDescriptorDict _propertySets;
    std::map<std::string, std::string> _reserved;
    std::set<std::string> _ignore;
};

class CommunicatorHelper
{
public:

    CommunicatorHelper(const Ice::CommunicatorPtr&, const CommunicatorDescriptorPtr&);
    CommunicatorHelper() { }
    virtual ~CommunicatorHelper() { }

    bool operator==(const CommunicatorHelper&) const;
    bool operator!=(const CommunicatorHelper&) const;

    virtual void getIds(std::multiset<std::string>&, std::multiset<Ice::Identity>&) const;
    
    void print(IceUtil::Output&) const;

protected:

    void printDbEnv(IceUtil::Output&, const DbEnvDescriptor&) const;
    void printObjectAdapter(IceUtil::Output&, const AdapterDescriptor&) const;
    void printPropertySet(IceUtil::Output&, const PropertySetDescriptor&) const;
    virtual std::string getProperty(const std::string&) const;

    void instantiateImpl(const CommunicatorDescriptorPtr&, const Resolver&) const;
    
    Ice::CommunicatorPtr _communicator;

private:

    CommunicatorDescriptorPtr _desc;
};

class ServiceHelper : public CommunicatorHelper
{
public:

    ServiceHelper(const Ice::CommunicatorPtr&, const ServiceDescriptorPtr&);
    ServiceHelper() { }

    bool operator==(const ServiceHelper&) const;
    bool operator!=(const ServiceHelper&) const;    

    ServiceDescriptorPtr getDescriptor() const;
    ServiceDescriptorPtr instantiate(const Resolver&, const PropertySetDescriptor&) const;

    void print(IceUtil::Output&) const;

protected:

    void instantiateImpl(const ServiceDescriptorPtr&, const Resolver&, const PropertySetDescriptor&) const;

private:
    
    ServiceDescriptorPtr _desc;
};

class ServerHelper : public CommunicatorHelper, public IceUtil::SimpleShared
{
public:

    ServerHelper(const Ice::CommunicatorPtr&, const ServerDescriptorPtr&);
    ServerHelper() { }

    bool operator==(const ServerHelper&) const;
    bool operator!=(const ServerHelper&) const;    

    ServerDescriptorPtr getDescriptor() const;
    virtual ServerDescriptorPtr instantiate(const Resolver&, const PropertySetDescriptor&) const;

    void print(IceUtil::Output&) const;
    void print(IceUtil::Output&, const std::string&, const std::string&) const;

protected:

    void printImpl(IceUtil::Output&, const std::string&, const std::string&) const;
    void instantiateImpl(const ServerDescriptorPtr&, const Resolver&, const PropertySetDescriptor&) const;

private:
    
    ServerDescriptorPtr _desc;
};
typedef IceUtil::Handle<ServerHelper> ServerHelperPtr;

class InstanceHelper
{
protected:

    InstanceHelper(const Ice::CommunicatorPtr&);

    std::map<std::string, std::string> instantiateParams(const Resolver&, 
							 const std::string&, 
							 const std::map<std::string, std::string>&,
							 const std::vector<std::string>&,
							 const std::map<std::string, std::string>&) const;

    Ice::CommunicatorPtr _communicator;
};


class ServiceInstanceHelper : public InstanceHelper
{
public:

    ServiceInstanceHelper(const Ice::CommunicatorPtr&, const ServiceInstanceDescriptor&);
    ServiceInstanceHelper(const Ice::CommunicatorPtr&);

    bool operator==(const ServiceInstanceHelper&) const;
    bool operator!=(const ServiceInstanceHelper&) const;

    ServiceInstanceDescriptor instantiate(const Resolver&) const;
    void getIds(std::multiset<std::string>&, std::multiset<Ice::Identity>&) const;

    void print(IceUtil::Output&) const;

private:
    
    ServiceInstanceDescriptor _definition;
    mutable ServiceHelper _service;
};


class IceBoxHelper : public ServerHelper
{
public:

    IceBoxHelper(const Ice::CommunicatorPtr&, const IceBoxDescriptorPtr&);
    IceBoxHelper() { }

    bool operator==(const IceBoxHelper&) const;
    bool operator!=(const IceBoxHelper&) const;    

    virtual ServerDescriptorPtr instantiate(const Resolver&, const PropertySetDescriptor&) const;

    virtual void getIds(std::multiset<std::string>&, std::multiset<Ice::Identity>&) const;

    void print(IceUtil::Output&) const;
    void print(IceUtil::Output&, const std::string&, const std::string&) const;

protected:

    void instantiateImpl(const IceBoxDescriptorPtr&, const Resolver&, const PropertySetDescriptor&) const;

private:
    
    IceBoxDescriptorPtr _desc;

    std::vector<ServiceInstanceHelper> _services;
};

class ServerInstanceHelper : public InstanceHelper
{
public:

    ServerInstanceHelper(const Ice::CommunicatorPtr&, const ServerInstanceDescriptor&, const Resolver&);
    ServerInstanceHelper(const Ice::CommunicatorPtr&, const ServerDescriptorPtr&, const Resolver&);
    ServerInstanceHelper(const Ice::CommunicatorPtr&);
    
    void operator=(const ServerInstanceHelper&);
    bool operator==(const ServerInstanceHelper&) const;
    bool operator!=(const ServerInstanceHelper&) const;

    std::string getId() const;
    ServerInstanceDescriptor getDefinition() const;
    ServerInstanceDescriptor getInstance() const;

    ServerDescriptorPtr getServerDefinition() const;
    ServerDescriptorPtr getServerInstance() const;

    void getIds(std::multiset<std::string>&, std::multiset<Ice::Identity>&) const;

private:

    void init(const ServerDescriptorPtr&, const Resolver&);

    const ServerInstanceDescriptor _definition;
    ServerInstanceDescriptor _instance;

    ServerHelperPtr _serverDefinition;
    ServerHelperPtr _serverInstance;
};

class NodeHelper
{
public:

    NodeHelper(const Ice::CommunicatorPtr&, const std::string&, const NodeDescriptor&, const Resolver&);
    NodeHelper(const Ice::CommunicatorPtr&);
    virtual ~NodeHelper() { }

    bool operator==(const NodeHelper&) const;
    bool operator!=(const NodeHelper&) const;

    NodeUpdateDescriptor diff(const NodeHelper&) const;
    void update(const NodeUpdateDescriptor&, const Resolver&);
    void instantiateServer(const ServerInstanceDescriptor&, const Resolver&);

    void getIds(std::multiset<std::string>&, std::multiset<std::string>&, std::multiset<Ice::Identity>&) const;
    const NodeDescriptor& getDescriptor() const;
    const NodeDescriptor& getInstance() const;
    void getServerInfos(const std::string&, std::map<std::string, ServerInfo>&) const;
    bool hasDistributions(const std::string&) const;
    bool hasServers() const;
    bool hasServer(const std::string&) const;
    void print(IceUtil::Output&) const;
    void printDiff(IceUtil::Output&, const NodeHelper&) const;
    void validate(const Resolver&) const;

private:

    NodeDescriptor instantiate(const Resolver&) const;

    Ice::CommunicatorPtr _communicator;
    std::string _name;
    NodeDescriptor _definition;    
    NodeDescriptor _instance;

    typedef std::map<std::string, ServerInstanceHelper> ServerInstanceHelperDict;
    ServerInstanceHelperDict _serverInstances;
    ServerInstanceHelperDict _servers;
};

class ApplicationHelper
{
public:

    ApplicationHelper(const Ice::CommunicatorPtr&, const ApplicationDescriptor&);

    ApplicationUpdateDescriptor diff(const ApplicationHelper&);
    void update(const ApplicationUpdateDescriptor&);
    void instantiateServer(const std::string&, const ServerInstanceDescriptor&);

    void getIds(std::set<std::string>&, std::set<std::string>&, std::set<Ice::Identity>&) const;
    const ApplicationDescriptor& getDescriptor() const;
    const ApplicationDescriptor& getInstance() const;
    TemplateDescriptor getServerTemplate(const std::string&) const;
    TemplateDescriptor getServiceTemplate(const std::string&) const;
    void getDistributions(DistributionDescriptor&, std::vector<std::string>&,const std::string& = std::string()) const;

    void print(IceUtil::Output&) const;
    void printDiff(IceUtil::Output&, const ApplicationHelper&) const;
    std::map<std::string, ServerInfo> getServerInfos() const;

private:

    void validate(const Resolver&) const;
    ApplicationDescriptor instantiate(const Resolver&) const;

    Ice::CommunicatorPtr _communicator;
    ApplicationDescriptor _definition;
    ApplicationDescriptor _instance;

    typedef std::map<std::string, NodeHelper> NodeHelperDict;
    NodeHelperDict _nodes;
};

bool descriptorEqual(const Ice::CommunicatorPtr&, const ServerDescriptorPtr&, const ServerDescriptorPtr&);

}

#endif

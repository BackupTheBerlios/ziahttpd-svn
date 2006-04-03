//
// ziis.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 10:34:58 2006 texane
// Last update Mon Apr  3 14:44:15 2006 
//



#ifndef ZIIS_HH
# define ZIIS_HH


// os specifics
#ifndef _WIN32
typedef int SOCKET;
#endif // !_WIN32


class IBuffer
{
public:
  virtual int Length() = 0;
  virtual const char* Str() = 0;
  virtual void Clear() = 0;
  virtual void Append(const char*, int) = 0;
  virtual IBuffer& operator=(IBuffer&) = 0;
  virtual IBuffer& operator+=(IBuffer&) = 0;
  virtual char& operator[](unsigned int) = 0;

  virtual ~IBuffer() {};
};


class IFS
{
public:
  virtual int IFSOpen(const char*, const char) = 0;
  virtual int IFSRead(int, long long, char*, int) = 0;
  virtual int IFSWrite(int, const char*, int) = 0;
  virtual void IFSClose(int) = 0;
  virtual long long IFSGetSize(int) = 0;
  virtual bool IFSRemove(const char*) = 0;

  virtual ~IFS() {};
};


class IModule
{
public:
  virtual bool ReadConfig(const char*) = 0;
  virtual const char* GetModuleName() = 0;
  virtual const char* GetModuleVersion() = 0;
  virtual void OnLoad(IFS*) = 0;

  virtual ~IModule() {};
};


class IConnection
{
public:
  virtual short GetPort() = 0;
  virtual const char* GetHost() = 0;
  virtual void* Accept(SOCKET) = 0;
  virtual int Recv(SOCKET, void*, char*, int) = 0;
  virtual int Send(SOCKET, void*, const char*, int) = 0;
  virtual void Close(SOCKET, void*) = 0;

  virtual ~IConnection() {};
};


class IInput
{
public:
  virtual const char* GetInput(const char*) = 0;
  virtual const char* GetInputMethod() = 0;
  virtual const char* GetInputLocation() = 0;
  virtual const char* GetInputHttpVersion() = 0;
  virtual const char* GetInputQueryString() = 0;
  virtual int GetClientIp() = 0;
  virtual int ReadPostEntity(char*, int) = 0;
  virtual bool GetNextHeader(char**, char**) = 0;

  virtual ~IInput() {};
};


class IOutput
{
public:
  virtual void SetOutput(const char*, const char*) = 0;
  virtual const char* GetOutput(const char*) = 0;
  virtual bool SendHeader() = 0;
  virtual int SendError(int) = 0;
  virtual int SendBuffer(const char*, int) = 0;
  virtual void SetStatusCode(int) = 0;

  virtual ~IOutput() {};
};


class ICompressor
{
public:
  virtual void* GetNewContext(const char*) = 0;
  virtual void DestroyContext(void*) = 0;
  virtual bool Compress(void*, IBuffer&, IBuffer&) = 0;
  virtual bool Decompress(void*, IBuffer&, IBuffer&) = 0;
  virtual const char** GetSupportedEncoding() = 0;

  virtual ~ICompressor() {};
};


class IDocumentGenerator
{
public:
  virtual void GenerateDocument(IInput&, const char*, IOutput&) = 0;
  virtual const char** GetSupportedMime() = 0;

  virtual ~IDocumentGenerator() {};
};


class IStreamModifier
{
public:
  virtual int GetPriority() = 0;
  virtual const char** GetSupportedContentType() = 0;
  virtual void Transform(IBuffer&, IBuffer&, IBuffer&, bool = false) = 0;

  virtual ~IStreamModifier() {};
};


#ifdef _WIN32
# define DLLEXPORT __declspec(dllexport)
#else
# define DLLEXPORT
#endif // !_WIN32

extern "C" DLLEXPORT IModule* GetNewInstance();


#endif // ! ZIIS_HH

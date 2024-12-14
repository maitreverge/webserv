# Classe de Logging pour Webserv
## 1. Utilisation de la classe Logger

**Prérequis**

La classe Logger suit le design pattern Singleton, ce qui signifie qu'il ne peut y avoir qu'une seule instance de cette classe au sein de l'application.

```c++
Logger::getInstance()
```
**2.1 Log avec message simple**
```c++
Logger::getInstance().log(ERROR, "select");
```

```bash
Thu, 24 Oct 2024 14:44:09 GMT: [ERROR] select
```

**2.2 Log avec contexte d'appel**
```c++
Logger::getInstance().log(INFO, "Request parsed", *this);
```

```bash
Thu, 24 Oct 2024 14:44:09 GMT: [INFO] Request parsed 10.12.7.8 56514 8
```

**N'hesitez pas a me dire si vous voulez une autre surcharge de methode!**

**FILTERING**
/**========================================================================
 *                             LOGGER
 *  LOG CONFIG: values to be changed in .cpp file:
 *?  Logger::Logger() : logToStdOut(1) 	=> 	0 to silence console log
 *?{
 *?	_logLevel[INFO] = 		1;			=>	0 to filter
 *?	_logLevel[DEBUG] = 		1;			=>	0 to filter
 *?	_logLevel[WARNING] = 	1;			=>	0 to filter
 *?	_logLevel[ERROR] = 		1;			=>	0 to filter
 *?	... 
 *========================================================================**/
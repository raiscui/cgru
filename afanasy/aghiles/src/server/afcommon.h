#pragma once

#include "../libafanasy/name_af.h"

#include "../libafsql/name_afsql.h"

#include "msgqueue.h"
#include "filequeue.h"
#include "dbupdatetaskqueue.h"
#include "dbactionqueue.h"
#include "cleanupqueue.h"
#include "logqueue.h"

class Core;

class AFCommon
{
public:
   AFCommon( Core * core);
   ~AFCommon();

   static void executeCmd( const std::string & cmd);             ///< Execute command.

/// Save string list, perform log file rotation;
   static void saveLog( const std::list<std::string> & log, const std::string & dirname, const std::string & filename, int rotate = 0);

   static bool writeFile( const char * data, const int length, const std::string & filename); ///< Write a file

//   static void catchDetached(); ///< Try to wait any child ( to prevent Zombie processes).

   inline static void MsgDispatchQueueRun() { if( MsgDispatchQueue) MsgDispatchQueue->run(); }
   inline static void FileWriteQueueRun()   { if( FileWriteQueue)   FileWriteQueue->run();   }
   inline static void CleanUpJobQueueRun()  { if( CleanUpJobQueue)  CleanUpJobQueue->run();  }
   inline static void OutputLogQueueRun()   { if( OutputLogQueue)   OutputLogQueue->run();   }
   inline static void DBUpTaskQueueRun()    { if( DBUpTaskQueue)    DBUpTaskQueue->run();    }
   inline static void DBUpdateQueueRun()    { if( DBUpdateQueue)    DBUpdateQueue->run();    }

   inline static bool QueueMsgDispatch( MsgAf* msg)       { if( MsgDispatchQueue ) return MsgDispatchQueue->pushMsg( msg);     else return false;}
   inline static bool QueueFileWrite( FileData* filedata) { if( FileWriteQueue   ) return FileWriteQueue->pushFile( filedata); else return false;}
   inline static bool QueueJobCleanUp( const JobAf * job) { if( CleanUpJobQueue  ) return CleanUpJobQueue->pushJob( job);      else return false;}

   inline static bool QueueLog(        const std::string & log) { if( OutputLogQueue) return OutputLogQueue->pushLog( log, LogData::Info  ); else return false;}
   inline static bool QueueLogError(   const std::string & log) { if( OutputLogQueue) return OutputLogQueue->pushLog( log, LogData::Error ); else return false;}
   inline static bool QueueLogErrno(   const std::string & log) { if( OutputLogQueue) return OutputLogQueue->pushLog( log, LogData::Errno ); else return false;}

   inline static void QueueDBAddItem(    const afsql::DBItem * item) { if( DBUpdateQueue ) DBUpdateQueue->addItem(    item );}
   inline static void QueueDBDelItem(    const afsql::DBItem * item) { if( DBUpdateQueue ) DBUpdateQueue->delItem(    item );}
   inline static void QueueDBUpdateItem( const afsql::DBItem * item, int attr = -1) { if( DBUpdateQueue ) DBUpdateQueue->updateItem( item, attr );}

   inline static void QueueDBUpdateTask_begin() { DBUpTaskQueue->lock(); }
   inline static bool QueueDBUpdateTask(  int JobId, int BlockNum, int TaskNum, const af::TaskProgress * Progress)
                         { return DBUpTaskQueue->pushTaskUp( JobId, BlockNum, TaskNum, Progress);}
   inline static void QueueDBUpdateTask_end() { DBUpTaskQueue->unlock(); }

   inline static void MsgDispatchQueueQuit() { if( MsgDispatchQueue) MsgDispatchQueue->quit(); }
   inline static void FileWriteQueueQuit()   { if( FileWriteQueue)   FileWriteQueue->quit();   }
   inline static void CleanUpJobQueueQuit()  { if( CleanUpJobQueue)  CleanUpJobQueue->quit();  }
   inline static void OutputLogQueueQuit()   { if( OutputLogQueue)   OutputLogQueue->quit();   }
   inline static void DBUpTaskQueueQuit()    { if( DBUpTaskQueue)    DBUpTaskQueue->quit();    }
   inline static void DBUpdateQueueQuit()    { if( DBUpdateQueue)    DBUpdateQueue->quit();    }

private:
   static MsgQueue          * MsgDispatchQueue;
   static FileQueue         * FileWriteQueue;
   static CleanUpQueue      * CleanUpJobQueue;
   static LogQueue          * OutputLogQueue;
   static DBUpdateTaskQueue * DBUpTaskQueue;
   static DBActionQueue     * DBUpdateQueue;

//   static bool detach();
};
Skydas
=========

Skydas is a ESL API and Angluar JS Panel for Freeswitch using PHP and ESL.
    
  - Uses Slim PHP and FS EventSocket Layer
  - Angualr JS for a Mod_Callcenter Web Interface
  
Panel
----
The Panel is all powered by Angular JS and is live updating

____

Installation
============
Please install the ESL PHP module from FreeSwitch Src on 1.2.stable see [PHP ESL](https://wiki.freeswitch.org/wiki/PHP_ESL)

```
Git clone to your web directory
git clone https://github.com/Symplicity/skydas

point a Apache / Nginx / Lighttpd host at it
cd skydas/api/
cp settings.inc.DIST settings.inc (Updated with your information)
cd ../panel/app/scripts
cp config.js.DIST config.js (Update with your API_HOST url)

load in browser supporting js at http://<url>/panel and http://<url>/api/
```

____


Api
----
  - Here is a list of commands, all data is returned in JSON
  - Require that all POST Requests are in JSON
  
**API Commands**

`/:queue/agents` -> (GET) Returns All Agents in a Given Queue

```
{
    "msg": {
        "1003@default": {
            "name": "Sue Smith",
            "status": "Available",
            "state": "Receiving",
            "no_answer_count": "1",
            "calls_answered": "16",
            "extension": "1003@default",
            "queue": "queue1@default",
            "queue_state": "Offering",
            "level": "1",
            "position": "1"
        }
    },
    "error": false,
    "status": 200
}
```

`/:queue/callers` -> (GET) Returns All Callers in Given Queue

```
{
    "msg": [
        {
            "queue": "queue1@default",
            "system": "single_box",
            "uuid": "ce2a7a0c-2ada-11e3-86e3-b1e2db555261",
            "session_uuid": "cacd7fc6-2ada-11e3-86da-b1e2db555261",
            "cid_number": "+19162785621",
            "cid_name": "+19162785621",
            "system_epoch": "1380660500",
            "joined_epoch": "1380660500",
            "rejoined_epoch": "0",
            "bridge_epoch": "1380660522",
            "abandoned_epoch": "0",
            "base_score": "0",
            "skill_score": "0",
            "serving_agent": "1009@default",
            "serving_system": "single_box",
            "state": "Answered"
        }
    ],
    "error": false,
    "status": 200
}
```

`/queues/fetch` -> (GET) Returns All Queues names in the System


```
{
    "msg": {
        "sales": {
            "agents": {},
            "callers": {}
        },
        "support": {
            "agents": {},
            "callers": {}
        }
    },
    "error": false,
    "status": 200
}
```

`/queues/status/:agent` -> (GET) Returns Agent State and Status in Mod_Callcenter

```
{
    "msg": {
        "status": "Available",
        "state": "Waiting"
    },
    "error": false,
    "status": 200
}
```

`/queues/dnd/:agent/:state` -> {on | off} (POST) Returns Array Containing Message

```
{
    "msg":"DND is Set for Agent",
    "error": false,
    "status": 200
}
```

`/validate/:queue` -> pass queue name returns true or false

```
{
        "validate": "true"
}
```

`/tiers` -> Returns Teirs for all queues

```
{
    "msg": [
        {
            "queue": "queue2@default",
            "agent": "1002@default",
            "state": "Ready",
            "level": "1",
            "position": "1"
        },
        {
            "queue": "queue1@default",
            "agent": "1002@default",
            "state": "No Answer",
            "level": "1",
            "position": "1"
        }
    ],
    "error": false,
    "status": 200
}
```

`/user/:exten` -> (GET) Returns username

```
{
    "msg": "Richard Genthner",
    "error": false,
    "status": 200
}
```

`/create/extension` -> (POST) requires the following JSON object -> Returns true or false

```
{
    "access_key": "my super key here",
    "action":"reloadxml",
    "user_id": "9999",
    "params": {
        "password": "123",
        "vm-password": "1234",
        "vm-email-all-messages": "true",
        "vm-notify-mailto": "true",
        "vm-mailto": "john.doe@example.com",
        "vm-attach-file": "true",
        "vm-message-ext": "wav"
    },
    "variables": {
        "toll_allow": "domestic,international,local",
        "accountcode": "9999",
        "user_context": "default",
        "effective_caller_id_name": "John Doe",
        "effective_caller_id_number": "9999",
        "outbound_caller_id_name": "$${outbound_caller_name}",
        "outbound_caller_id_number": "$${outbound_caller_id}",
        "directory_full_name": "John Doe"
    }
}
```

`/connect` -> (POST) requires the following JSON object

```
    Comming Soon!
```

`/voicemail/greeting` -> (POST) requires the following JSON object

```
{
    "access_key": "my super key",
    "exten": "1009",
    "greeting_number": "1"
} 
```

`/calls` -> Returns a JSON object of calls on the system

```
{
    "msg": [
        {
            "uuid,direction,created,created_epoch,name,state,cid_name,cid_num,ip_addr,dest,presence_id,presence_data,callstate,callee_name,callee_num,callee_direction,call_uuid,hostname,sent_callee_name,sent_callee_num,b_uuid,b_direction,b_created,b_created_epoch,b_name,b_state,b_cid_name,b_cid_num,b_ip_addr,b_dest,b_presence_id,b_presence_data,b_callstate,b_callee_name,b_callee_num,b_callee_direction,b_sent_callee_name,b_sent_callee_num,call_created_epoch": "fb2e5b04-3c00-11e3-96f0-1ff78d1619a2,inbound,2013-10-23 12:34:26,1382546066,sofia/internal/1000@fsbox.example.com:5060,CS_EXECUTE,John Doe,1000,24.2.55.181,1000,1000@fsbox.example.com,,UNHOLD,Outbound Call,1003,SEND,fb2e5b04-3c00-11e3-96f0-1ff78d1619a2,fsbox.example.com,Outbound Call,1003,fb530f9e-3c00-11e3-970e-1ff78d1619a2,outbound,2013-10-23 12:34:26,1382546066,sofia/internal/sip:1000@10.122.1.152:56884,CS_EXCHANGE_MEDIA,John Doe<1000>,8243,24.2.55.181,8233,1000@fsbox.example.com,,UNHOLD,Outbound Call,1000,SEND,User<1000>,1000,1382546068"
        }
    ],
    "error": false,
    "status": 200
}
```

`/tools/caller/:password/:cid` -> Returns a JSON object of caller id information

```
        {"msg":"WISCASSET,ME 2175550005","error":false,"status":200 }
```

`/monitor/:stat` -> Returns a Json object of a Digit

Author
----
Richard 'Moose' Genthner <rgenthner@symplicity.com>

License
--------

BSD

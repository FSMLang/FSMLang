typedef enum
{
	ext1
	, ext2
	, ext3
	, ext4
	, ext5
} external_e;

typedef enum
{
	exts51
	, exts52
	, exts53
	, exts54
} externals5_e;

typedef enum
{
	extsse1
	, extsse2
} externals52_e;

typedef enum
{
	exts541
	, exts542
	, exts543
} externals54_e;

typedef enum
{
	exts5421
	, exts5422
	, exts5423
} externals542_e;

typedef enum
{
	exts1
	, exts2
} externalss_e;

typedef struct _header_          header_t,          *pheader_t;
typedef union  _body_            body_t,            *pbody_t;
typedef struct _complex_event_   complex_event_t,   *pcomplex_event_t;
typedef struct _event_4_         event_4_t,         *pevent_4_t;
typedef struct _event_5_         event_5_t,         *pevent_5_t;
typedef struct _event_52_        event_52_t,        *pevent_52_t;
typedef struct _event_54_        event_54_t,        *pevent_54_t;
typedef struct _event_542_       event_542_t,       *pevent_542_t;

struct _header_
{
   externals5_e head;
};

struct _event_4_
{
   externalss_e e;
};

struct _event_52_
{
   externals52_e e;
};

struct _event_54_
{
   externals54_e e;
};

struct _event_542_
{
   externals542_e e;
};

union _body_
{
   event_52_t  event_52;
   event_54_t  event_54;
   event_542_t event_542;
};

struct _complex_event_
{
   header_t head;
   body_t   body;
};


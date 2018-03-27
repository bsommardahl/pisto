module DatabaseInfo: {
  type t = {
    .
    "db_name": string,
    "doc_count": int,
    "update_seq": int,
  };
};

module RevResponse: {
  type t = {
    .
    "ok": bool,
    "id": string,
    "rev": string,
  };
};

module QueryBuilder: {
  type t;
  type queryT;
  [@bs.obj]
  external query :
    (
      ~selector: Js.t({..}),
      ~fields: option(array(string))=?,
      ~sort: option(array(string))=?,
      unit
    ) =>
    queryT =
    "";
};

type existing('existing) =
  {
    ..
    "_id": string,
    "_rev": string,
  } as 'existing;

type docArray('existing) = {. "docs": array('existing)};

type t('existing, 'fresh) = {
  info: t('existing, 'fresh) => Js.Promise.t(DatabaseInfo.t),
  destroy: t('existing, 'fresh) => Js.Promise.t(unit),
  put:
    (existing('existing), t('existing, 'fresh)) =>
    Js.Promise.t(RevResponse.t),
  get: (string, t('existing, 'fresh)) => Js.Promise.t('existing),
  post: ('fresh, t('existing, 'fresh)) => Js.Promise.t(RevResponse.t),
  /* find:
    (QueryBuilder.queryT, t('existing, 'fresh)) =>
    Js.Promise.t(docArray('existing)), */
  remove: ('existing, t('existing, 'fresh)) => Js.Promise.t(RevResponse.t),
  closeConnection: t('existing, 'fresh) => Js.Promise.t(unit),
};

module type init = {let connect: string => t('existing, 'fresh);};
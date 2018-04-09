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

type t;

let info: t => Js.Promise.t(DatabaseInfo.t);

let destroy: t => Js.Promise.t(unit);

let put: ('a, t) => Js.Promise.t(RevResponse.t);

let remove: ('a, t) => Js.Promise.t(RevResponse.t);

let post: ('fresh, t) => Js.Promise.t(RevResponse.t);

let get: (string, t) => Js.Promise.t('existing');

let find: (QueryBuilder.queryT, t) => Js.Promise.t('a);

let closeConnection: t => Js.Promise.t(unit);

let createIndex: ('a, t) => unit;

let connect: string => t;

module type init = {let connect: string => t;};
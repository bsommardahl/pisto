open PouchDbInterface;

type t;

type idObj('a) = {.. "_id": string} as 'a;

[@bs.send] external info : t => Js.Promise.t(DatabaseInfo.t) = "";

[@bs.send] external destroy : t => Js.Promise.t(unit) = "";

[@bs.send.pipe: t]
external put : idObj('a) => Js.Promise.t(RevResponse.t) = "";

[@bs.send.pipe: t]
external remove : idObj('a) => Js.Promise.t(RevResponse.t) = "";

[@bs.send.pipe: t] external post : 'a => Js.Promise.t(RevResponse.t) = "";

[@bs.send.pipe: t] external get : string => Js.Promise.t(Js.t('a)) = "";

[@bs.send.pipe: t]
external find : QueryBuilder.queryT => Js.Promise.t(Js.t('a)) = "";

[@bs.send] external closeConnection : t => Js.Promise.t(unit) = "close";

[@bs.send.pipe: t] external createIndex : 'a => unit = "";

type pluginModule;

[@bs.module] external findPluginModule : pluginModule = "pouchdb-find";

[@bs.module "pouchdb"] external plugin : pluginModule => unit = "";

[@bs.module] [@bs.new] external connect : string => t = "pouchdb";

let connect = (dbNameOrUrl: string) => {
  plugin(findPluginModule);
  let db = connect(dbNameOrUrl);
  db;
};
module Database = {
  type dbOptions = {
    .
    "auth": {
      .
      "username": string,
      "password": string,
    },
  };
  type connectionConfig = {
    host: string,
    options: dbOptions,
  };
  type syncOptions = {
    .
    "live": bool,
    "retry": bool,
  };
  type syncConnectionConfig = {
    host: string,
    options: dbOptions,
    syncOptions,
  };
  type pouchDbConfig = {
    local: connectionConfig,
    remote: option(syncConnectionConfig),
  };
  let livePouchDbConfig = {
    local: {
      host: "",
      options: {
        "auth": {
          "username": "",
          "password": "",
        },
      },
    },
    remote:
      Some({
        host: SyncDbConfigTemp.info##url ++ "/",
        options: {
          "auth": {
            "username": SyncDbConfigTemp.info##username,
            "password": SyncDbConfigTemp.info##password,
          },
        },
        syncOptions: {
          "live": true,
          "retry": true,
        },
      }),
  };
};
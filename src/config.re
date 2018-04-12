let setValue = (key: string, value: string) =>
  Dom.Storage.localStorage |> Dom.Storage.setItem(key, value);

let getValue = (key: string) =>
  Dom.Storage.localStorage |> Dom.Storage.getItem(key);

module Database = {
  type dbOptions = {
    .
    "auth": {
      .
      "username": string,
      "password": string,
    },
    "auto_compaction": bool,
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
  let toSyncConnectionConfig =
      (configDelimited: string)
      : syncConnectionConfig => {
    let syncArr = configDelimited |> Js.String.split("||");
    let config = {
      host: syncArr[0],
      options: {
        "auth": {
          "username": syncArr[1],
          "password": syncArr[2],
        },
        "auto_compaction": true,
      },
      syncOptions: {
        "live": syncArr[3] === "true" ? true : false,
        "retry": syncArr[4] === "true" ? true : false,
      },
    };
    config;
  };
  let toString = (config: syncConnectionConfig) : string =>
    [|
      config.host,
      config.options##auth##username,
      config.options##auth##password,
      config.syncOptions##live ? "true" : "false",
      config.syncOptions##retry ? "true" : "false",
    |]
    |> Js.Array.joinWith("||");
  let syncKey = "sync_config";
  let defaultLocalConnection = {
    host: "",
    options: {
      "auth": {
        "username": "",
        "password": "",
      },
      "auto_compaction": true,
    },
  };
  let livePouchDbConfig = {
    let config = {
      local: defaultLocalConnection,
      remote:
        switch (getValue(syncKey)) {
        | None => None
        | Some(configDelimited) =>
          Some(configDelimited |> toSyncConnectionConfig)
        },
    };
    config;
  };
};
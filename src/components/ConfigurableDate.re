let now = () =>
  switch (Config.App.get().now) {
  | None => Date.now()
  | Some(d) => d
  };
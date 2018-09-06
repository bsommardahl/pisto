let addTime = (now, t): Date.t => {
  let n = Js.Date.fromFloat(now);
  Js.Date.fromFloat(t)
  ->(
      Js.Date.setHoursMS(
        ~hours=Js.Date.getHours(n),
        ~minutes=Js.Date.getMinutes(n),
        ~seconds=Js.Date.getSeconds(n),
        (),
      )
    );
};

let now = () =>
  switch (Config.App.get().now) {
  | None => Date.now()
  | Some(d) => d |> addTime(Date.now())
  };

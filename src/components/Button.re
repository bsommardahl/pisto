let component = ReasonReact.statelessComponent("Button");

let make =
    (
      ~icon=?,
      ~label=?,
      ~onClick=_ => (),
      ~_type="button",
      ~disabled=false,
      ~local=false,
      ~className="",
      _children,
    ) => {
  ...component,
  render: _self =>
    <button
      _type
      onClick=(_ => onClick(label))
      disabled
      className=("card " ++ className)>
      (
        switch (icon) {
        | Some(icon) => <i className=("fa fa-" ++ icon) />
        | None => ReasonReact.nullElement
        }
      )
      (
        switch (label) {
        | Some(label) =>
          ReactUtils.s(local ? label |> Lang.translate : label)
        | None => ReasonReact.nullElement
        }
      )
    </button>,
};
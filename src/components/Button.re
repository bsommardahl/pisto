let component = ReasonReact.statelessComponent("Button");

let make =
    (
      ~iconClass=?,
      ~label=?,
      ~subLabel=?,
      ~onClick=_ => (),
      ~type_="button",
      ~disabled=false,
      ~local=false,
      ~className="",
      _children,
    ) => {
  ...component,
  render: _self =>
    <button
      type_
      onClick={_ => onClick(label)}
      disabled
      className={"card " ++ className}>
      {
        switch (iconClass) {
        | Some(iconClass) => <i className=iconClass />
        | None => ReasonReact.null
        }
      }
      {
        switch (label) {
        | Some(label) =>
          ReactUtils.s(local ? label |> Lang.translate : label)
        | None => ReasonReact.null
        }
      }
      {
        switch (subLabel) {
        | Some(subLabel) =>
          <div className="sub-label"> {ReasonReact.string(subLabel)} </div>
        | None => ReasonReact.null
        }
      }
    </button>,
};
let component = ReasonReact.statelessComponent("Button");

let make =
    (
      ~label: string,
      ~onClick=(_) => (),
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
      onClick=((_) => onClick(label))
      disabled
      className=("card " ++ className)>
      (ReactUtils.s(local ? label |> Lang.translate : label))
    </button>,
};
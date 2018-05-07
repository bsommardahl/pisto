let component = ReasonReact.statelessComponent("Button");

let make =
    (
      ~label: string,
      ~onClick,
      ~disabled=false,
      ~local=false,
      ~className="",
      _children,
    ) => {
  ...component,
  render: _self =>
    <button
      onClick=((_) => onClick(label))
      disabled=(disabled ? Js.true_ : Js.false_)
      className=("card " ++ className)>
      (ReactUtils.s(local ? label |> Lang.translate : label))
    </button>,
};
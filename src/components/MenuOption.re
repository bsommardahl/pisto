let component = ReasonReact.statelessComponent("MenuOption");

let make = (~messageKey: string, ~path: string, _children) => {
  ...component,
  render: _self =>
    <div className="menu-option" onClick={_ => ReasonReact.Router.push(path)}>
      {ReactUtils.s(messageKey |> Lang.translate)}
    </div>,
};

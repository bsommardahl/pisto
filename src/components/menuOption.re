let component = ReasonReact.statelessComponent("MenuOption");

let make = (~title: string, ~path: string, _children) => {
  ...component,
  render: _self =>
    <div
      className="menu-option" onClick=((_) => ReasonReact.Router.push(path))>
      (ReactUtils.s(title))
    </div>,
};
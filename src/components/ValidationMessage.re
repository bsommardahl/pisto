open ReactUtils;

let component = ReasonReact.statelessComponent("ValidationMessage");

let make = (~message="Requerido", ~hidden: bool, _children) => {
  ...component,
  render: _self =>
    hidden ? <span /> : <span className="invalid"> (s(message)) </span>,
};
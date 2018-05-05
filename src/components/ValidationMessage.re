open ReactUtils;

let component = ReasonReact.statelessComponent("ValidationMessage");

let make = (~key="validation.required", ~hidden: bool, _children) => {
  ...component,
  render: _self =>
    hidden ? <span /> : <span className="invalid"> (sloc(key)) </span>,
};
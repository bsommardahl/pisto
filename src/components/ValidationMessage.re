open ReactUtils;

let component = ReasonReact.statelessComponent("ValidationMessage");

let make = (~messageKey="validation.required", ~hidden: bool, _children) => {
  ...component,
  render: _self =>
    hidden ? <span /> : <span className="invalid"> (sloc(messageKey)) </span>,
};
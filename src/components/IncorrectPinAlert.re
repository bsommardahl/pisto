let component = ReasonReact.statelessComponent("IncorrectPinAlert");

let make = (~isOpen=false, ~toggle, ~label: string, _children) => {
  ...component,
  render: _self =>
    <BsReactstrap.Alert className="alert" isOpen toggle>
      (ReactUtils.sloc(label))
    </BsReactstrap.Alert>,
};
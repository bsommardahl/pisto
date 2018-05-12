let component = ReasonReact.statelessComponent("TagCard");

/* Here, the button should push to /NewOrder?customerName=john and the view changes */
let make = (~tag: string, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <Button
      key=tag
      onClick=(_ev => onSelect(tag))
      className="tag-card"
      label=tag
    />,
};
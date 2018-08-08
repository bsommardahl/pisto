[%bs.raw {|require('./app.css')|}];
[%bs.raw {|require('moment')|}];
[%bs.raw {|require('moment/locale/es-do')|}];

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <div className="App"> <AppHeader /> <CafeRouter /> </div>,
};
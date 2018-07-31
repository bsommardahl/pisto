[%bs.raw {|require('react-datetime/css/react-datetime.css')|}];
[@bs.module] external reactClass : ReasonReact.reactClass = "react-datetime";

[@bs.obj]
external makeProps :
  (
    ~className: string=?,
    ~locale: string=?,
    ~timeFormat: bool=?,
    ~input: bool=?,
    ~value: Js.Date.t=?,
    ~onBlur: MomentRe.Moment.t => unit=?,
    ~onFocus: ReactEventRe.Focus.t => unit=?,
    ~onChange: MomentRe.Moment.t => unit=?,
    unit
  ) =>
  _ =
  "";

let make =
    (
      ~className: option(string)=?,
      ~locale: option(string)=?,
      ~value: option(Js.Date.t)=?,
      ~timeFormat: option(bool)=?,
      ~input: option(bool)=?,
      ~onBlur: option(MomentRe.Moment.t => unit)=?,
      ~onFocus: option(ReactEventRe.Focus.t => unit)=?,
      ~onChange: option(MomentRe.Moment.t => unit)=?,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~className?,
        ~locale?,
        ~value?,
        ~timeFormat?,
        ~input?,
        ~onBlur?,
        ~onFocus?,
        ~onChange?,
        (),
      ),
    children,
  );
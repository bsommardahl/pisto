type t = {
  subTotal: Money.t,
  discount: Money.t,
  tax: Money.t,
  total: Money.t,
  on: Date.t,
  by: string,
};

let toJs = (p: t) => {
  "subTotal": p.subTotal,
  "discount": p.discount,
  "tax": p.tax,
  "total": p.total,
  "on": p.on,
  "by": p.by,
};

let fromJs = p => {
  subTotal: p##subTotal,
  discount: p##discount,
  tax: p##tax,
  total: p##total,
  on: p##on,
  by: p##by,
};
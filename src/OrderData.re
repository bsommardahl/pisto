module Paid = {
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
};

module Order = {
  type orderItem = {
    sku: string,
    name: string,
    suggestedPrice: Money.t,
    addedOn: float,
    salePrice: Money.t,
    taxCalculation: Tax.taxCalculationMethod,
  };
  type updateOrder = {
    id: string,
    customerName: string,
    orderItems: list(orderItem),
    discounts: list(Discount.t),
    paid: option(Paid.t),
  };
  type order = {
    id: string,
    customerName: string,
    paid: option(Paid.t),
    orderItems: list(orderItem),
    discounts: list(Discount.t),
    createdOn: float,
    lastUpdated: option(float),
    removed: bool,
  };
  type orderVm = {
    id: option(string),
    customerName: string,
    orderItems: list(orderItem),
    discounts: list(Discount.t),
    paid: option(Paid.t),
    createdOn: float,
    lastUpdated: option(float),
    removed: bool,
  };
  type newOrder = {
    customerName: string,
    orderItems: list(orderItem),
    discounts: list(Discount.t),
  };
};
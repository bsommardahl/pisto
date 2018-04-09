open OrderData;

type orderItemTotals = {
  subTotal: float,
  tax: float,
  total: float,
  discounts: float,
};

type roundedTotals = {
  subTotal: Money.t,
  tax: Money.t,
  total: Money.t,
  discounts: Money.t,
};

type totalCalculator =
  (Money.t, list(Discount.t), Order.orderItem) => orderItemTotals;

let calcDiscount =
    (item: Order.orderItem, discounts: list(Discount.t))
    : float =>
  Belt.List.reduce(
    discounts,
    0.,
    (acc, dis) => {
      let taxRate = float_of_int(dis.percent) /. 100.;
      let disc = float_of_int(item.suggestedPrice) /. 100. *. taxRate;
      acc +. disc;
    },
  );

let totalFirstCalculator: totalCalculator =
  (taxPercent: int, discounts: list(Discount.t), item: Order.orderItem) => {
    let discountCalc = discounts |> calcDiscount(item);
    let total = float_of_int(item.suggestedPrice) /. 100. -. discountCalc;
    let taxRate = (100. +. float_of_int(taxPercent)) /. 100.;
    let subTotal = total /. taxRate;
    let tax = total -. subTotal;
    {subTotal, discounts: discountCalc, tax, total};
  };

let subTotalFirstCalculator: totalCalculator =
  (taxPercent: int, discounts: list(Discount.t), item: Order.orderItem) => {
    let discountCalc = discounts |> calcDiscount(item);
    let taxRate = float_of_int(taxPercent) /. 100.;
    let subTotal = float_of_int(item.suggestedPrice) /. 100. -. discountCalc;
    let tax = subTotal *. taxRate;
    let total = subTotal +. tax;
    {subTotal, discounts: discountCalc, tax, total};
  };

let exemptCalculator: totalCalculator =
  (_tax, discounts: list(Discount.t), item: Order.orderItem) => {
    let discountCalc = discounts |> calcDiscount(item);
    let subTotal = float_of_int(item.suggestedPrice) /. 100. -. discountCalc;
    {subTotal, discounts: discountCalc, tax: 0., total: subTotal};
  };

let getTotals =
    (discounts: list(Discount.t), orderItems: list(Order.orderItem))
    : roundedTotals => {
  let itemTotals =
    orderItems
    |> List.map(item =>
         switch (Order.(item.taxCalculation)) {
         | Tax.TotalFirst(rate) =>
           item |> totalFirstCalculator(rate, discounts)
         | Tax.SubTotalFirst(rate) =>
           item |> subTotalFirstCalculator(rate, discounts)
         | Tax.Exempt => item |> exemptCalculator(0, discounts)
         }
       );
  let subTotal =
    Belt.List.reduce(itemTotals, 0., (acc, totals) => acc +. totals.subTotal);
  let discountCalc =
    Belt.List.reduce(itemTotals, 0., (acc, totals) =>
      acc +. totals.discounts
    );
  let tax =
    Belt.List.reduce(itemTotals, 0., (acc, totals) => acc +. totals.tax);
  let rounded = (i: float) : int => {
    let r = i *. 100. |> Js.Math.round;
    r |> int_of_float;
  };
  let total = rounded(subTotal) + rounded(tax);
  {
    subTotal: rounded(subTotal),
    tax: rounded(tax),
    discounts: rounded(discountCalc),
    total,
  };
};